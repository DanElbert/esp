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
  _mqtt_announce_topic({})
{

  if (GLOBAL_ESP_BUG) {
    Serial.println("ERROR:: Multiple EspBug objects were created; THIS CAN NOT CURRENTLY BE DONE");
    return;
  }

  GLOBAL_ESP_BUG = this;

  // Generate Full Version
  strncpy(_full_version, _bug_type, sizeof(_full_version));
  strcat(_full_version, "-");
  strcat(_full_version, _version);

  // Copy in the Bug ID
  strncpy(_id, secrets->getBugId(), sizeof(_id));

  // Generate Bug Name
  strncpy(_name, _bug_type, sizeof(_name));
  strcat(_name, "_");
  strcat(_name, _id);

  // MQTT announce
  buildMqttTopic(_mqtt_announce_topic, "announce");

  // MQTT ping
  buildMqttTopic(_mqtt_announce_topic, "ping");

  // MQTT update
  buildMqttTopic(_mqtt_announce_topic, "update");

  // Setup MQTT
  _mqtt.setServer(_secrets->getMqttServer(), _secrets->getMqttPort());
  _mqtt.setCallback(mqttCallback);
}

void EspBug::begin() {
  WiFi.hostname(_name);
  WiFi.mode(WIFI_STA);
  WiFi.begin(_secrets->getWifiSsid(), _secrets->getWifiPassword());

  while (WiFi.status() != WL_CONNECTED) {
    delay(250);
  }
}

void EspBug::update() {

}

void EspBug::insertBugId(char* str) {
  char* subStr = strstr(str, "XXX");

  if (subStr) {
    strncpy(subStr, _id, 3);
  }
}

void EspBug::buildMqttTopic(char* topic, char* name) {
  strncpy(topic, _bug_type, sizeof(topic));
  strcat(topic, "/");
  strcat(topic, name);
  strcat(topic, "/");
  strcat(topic, _id);
}

void EspBug::mqttCallback(char* topic, byte* payload, unsigned int length) {
  strstr(topic, GLOBAL_ESP_BUG->_mqtt_update_topic);
}

void EspBug::mqttReconnect() {
  // Loop until we're reconnected
  // while (!_mqtt.connected()) {
  //   #if defined( DEBUG )
  //   Serial.print("Attempting MQTT connection...");
  //   #endif
  //   // Attempt to connect
  //   if (_mqtt.connect(_name, _secrets->getMqttUser(), _secrets->getMqttPassword())) {
  //     #if defined( DEBUG )
  //     Serial.println("connected");
  //     #endif
  //     // Once connected, publish an announcement...
  //     strcpy(announce_buffer, "connected; version ");
  //     strcat(announce_buffer, current_version);
  //     client.publish(mqtt_announce_topic, announce_buffer);
  //     // ... and resubscribe
  //     client.subscribe(mqtt_ping_topic);
  //     client.subscribe(mqtt_update_topic);
  //   } else {
  //     #if defined( DEBUG )
  //     Serial.print("failed, rc=");
  //     Serial.print(_mqtt.state());
  //     Serial.println(" try again in 5 seconds");
  //     #endif
  //     // Wait 5 seconds before retrying
  //     delay(5000);
  //   }
  // }
}
