#include "EspBug.h"

EspBug::EspBug(const char* bug_type, const char* version, Secrets* secrets):
  _wifi(),
  _mqtt(_wifi),
  _bug_type(bug_type),
  _version(version),
  _secrets(secrets),
  _name({}),
  _full_version({}),
  _id({}),
  _mqtt_announce_topic({}),
  _messageBuffer({}),
  _shouldUpdate(false)
{

  if (GLOBAL_ESP_BUG) {
    Serial.println("ERROR:: Multiple EspBug objects were created; THIS CAN NOT CURRENTLY BE DONE");
    return;
  }

  GLOBAL_ESP_BUG = this;
}

void EspBug::begin() {
  // Generate Full Version
  strncpy(_full_version, _bug_type, sizeof(_full_version));
  strcat(_full_version, "-");
  strcat(_full_version, _version);

  // Copy in the Bug ID
  strncpy(_id, _secrets->getBugId(), sizeof(_id));

  // Generate Bug Name
  strncpy(_name, _bug_type, sizeof(_name));
  strcat(_name, "_");
  strcat(_name, _id);

  // MQTT announce
  buildMqttTopic(_mqtt_announce_topic, "announce");

  // MQTT ping
  buildMqttTopic(_mqtt_ping_topic, "ping");

  // MQTT update
  buildMqttTopic(_mqtt_update_topic, "update");

  // Setup MQTT
  _mqtt.setServer(_secrets->getMqttServer(), _secrets->getMqttPort());
  _mqtt.setCallback(GLOBAL_MQTT_CALLBACK);

  ESPhttpUpdate.rebootOnUpdate(false);

  DEBUG_PRINT("version:");
  DEBUG_PRINT(_full_version);
  DEBUG_PRINT("ESP ID: ");
  DEBUG_PRINT(ESP.getChipId());
  DEBUG_PRINT("name:");
  DEBUG_PRINT(_name);
  DEBUG_PRINT("topics:");
  DEBUG_PRINT(_mqtt_announce_topic);
  DEBUG_PRINT(_mqtt_ping_topic);
  DEBUG_PRINT(_mqtt_update_topic);


  WiFi.hostname(_name);
  WiFi.mode(WIFI_STA);
  DEBUG_PRINT("Connecting...");
  WiFi.begin(_secrets->getWifiSsid(), _secrets->getWifiPassword());

  while (WiFi.status() != WL_CONNECTED) {
    delay(250);
  }
  DEBUG_PRINT("Connected");
}

void EspBug::update() {
  mqttReconnect();
  _mqtt.loop();

  if (_shouldUpdate) {
    _mqtt.publish(_mqtt_announce_topic, "checking for updates...");
    t_httpUpdate_return ret = ESPhttpUpdate.update(_secrets->getUpdateUrl(), _full_version);

    switch(ret) {
        case HTTP_UPDATE_FAILED:
            Serial.printf("HTTP_UPDATE_FAILD Error (%d): %s", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
            break;

        case HTTP_UPDATE_NO_UPDATES:
            Serial.println("HTTP_UPDATE_NO_UPDATES");
            break;

        case HTTP_UPDATE_OK:
            Serial.println("HTTP_UPDATE_OK");
            ESP.restart();
            break;
    }

    _shouldUpdate = false;
  }
}

void EspBug::insertBugId(char* str) {
  char* subStr = strstr(str, "XXX");

  if (subStr) {
    strncpy(subStr, _id, 3);
  }
}

void EspBug::buildMqttTopic(char* topic, char* name) {
  strcpy(topic, _bug_type);
  strcat(topic, "/");
  strcat(topic, name);
  strcat(topic, "/");
  strcat(topic, _id);
}

void EspBug::mqttCallback(char* topic, byte* payload, unsigned int length) {
  DEBUG_PRINT("MQTT:");
  DEBUG_PRINT(topic);
  if (strcmp(topic, _mqtt_ping_topic) == 0) {
    _mqtt.publish(_mqtt_announce_topic, _full_version);
  }

  if (strcmp(topic, _mqtt_update_topic) == 0) {
    _shouldUpdate = true;
  }
}

void EspBug::mqttReconnect() {
  //Loop until we're reconnected
  while (!_mqtt.connected()) {

    DEBUG_PRINT("Attempting MQTT connection...");

    if (_mqtt.connect(_name, _secrets->getMqttUser(), _secrets->getMqttPassword())) {
      DEBUG_PRINT("connected");

      // Once connected, publish an announcement...
      strcpy(_messageBuffer, "connected; version ");
      strcat(_messageBuffer, _full_version);
      _mqtt.publish(_mqtt_announce_topic, _messageBuffer);
      // ... and resubscribe
      _mqtt.subscribe(_mqtt_ping_topic);
      _mqtt.subscribe(_mqtt_update_topic);
    } else {
      DEBUG_PRINT("failed, rc=");
      DEBUG_PRINT(_mqtt.state());
      DEBUG_PRINT(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
