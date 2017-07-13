#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
#include <PubSubClient.h>
#include <secrets.h>
#include <esp_bug.h>
#include "Doorway.h"

// #define DEBUG

void reconnect();
void callback(char*, byte*, unsigned int);
void set_id(char*, const char*);

const char* update_url = "";
const char* current_version = "door_bug-0.0.22";
char announce_buffer[75];

// 3 Digit ID; Unique per device (set based on ESP id)
const char* door_bug_id = "000";

const char* wifi_ssid     = "";
const char* wifi_password = "";
const char* mqtt_server = "";

const char* mqtt_user = "";
const char* mqtt_password = "";

char mqtt_client_name[] = "door_bug_XXX";
char mqtt_traversal_topic[] = "door_bug/traversal/XXX";
char mqtt_announce_topic[] = "door_bug/announce/XXX";
char mqtt_ping_topic[] = "door_bug/ping/XXX";
char mqtt_update_topic[] = "door_bug/update/XXX";

WiFiClient espClient;
PubSubClient client(espClient);
Doorway door(13, 14, &client, mqtt_traversal_topic);
bool shouldUpdate = false;

void setup() {
  Serial.begin(115200);
  delay(100);

  switch(ESP.getChipId()) {
    case 2:
      door_bug_id = "001";
      break;
    case 1:
      door_bug_id = "002";
      break;
  }

  pinMode(0, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, INPUT_PULLUP);
  pinMode(14, INPUT_PULLUP);

  set_id(mqtt_client_name, door_bug_id);
  set_id(mqtt_traversal_topic, door_bug_id);
  set_id(mqtt_announce_topic, door_bug_id);
  set_id(mqtt_ping_topic, door_bug_id);
  set_id(mqtt_update_topic, door_bug_id);

  #if defined( DEBUG )
  Serial.println();
  Serial.print("Firmware Version: ");
  Serial.println(current_version);
  Serial.print("ESP ID: ");
  Serial.println(ESP.getChipId());
  Serial.print("Client ID: ");
  Serial.println(mqtt_client_name);
  Serial.print("Connecting to ");
  Serial.println(wifi_ssid);
  #endif
  WiFi.hostname(mqtt_client_name);
  WiFi.mode(WIFI_STA);
  WiFi.begin(wifi_ssid, wifi_password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    #if defined( DEBUG )
    Serial.print(".");
    #endif
  }

  #if defined( DEBUG )
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  #endif

  client.setServer(mqtt_server, 0);
  client.setCallback(callback);

  analogWriteFreq(38000);
  analogWriteRange(127);
  analogWrite(12, 63);

  ESPhttpUpdate.rebootOnUpdate(false);
}

// Replaces the last 3 characters of value with id
void set_id(char* value, const char* id) {
  while (*value != '\0')
    value++;

  *(value - 3) = id[0];
  *(value - 2) = id[1];
  *(value - 1) = id[2];
}

void callback(char* topic, byte* payload, unsigned int length) {
  #if defined( DEBUG )
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  #endif

  if (strcmp(topic, mqtt_ping_topic) == 0) {
    client.publish(mqtt_announce_topic, current_version);
  }

  if (strcmp(topic, mqtt_update_topic) == 0) {
    shouldUpdate = true;
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    #if defined( DEBUG )
    Serial.print("Attempting MQTT connection...");
    #endif
    // Attempt to connect
    if (client.connect(mqtt_client_name, mqtt_user, mqtt_password)) {
      #if defined( DEBUG )
      Serial.println("connected");
      #endif
      // Once connected, publish an announcement...
      strcpy(announce_buffer, "connected; version ");
      strcat(announce_buffer, current_version);
      client.publish(mqtt_announce_topic, announce_buffer);
      // ... and resubscribe
      client.subscribe(mqtt_ping_topic);
      client.subscribe(mqtt_update_topic);
    } else {
      #if defined( DEBUG )
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      #endif
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  door.loop();

  int lightState1 = digitalRead(14);
  int lightState2 = digitalRead(13);

  if (lightState1 == HIGH) {
    digitalWrite(0, LOW);
  } else {
    digitalWrite(0, HIGH);
  }

  if (lightState2 == HIGH) {
    digitalWrite(2, LOW);
  } else {
    digitalWrite(2, HIGH);
  }

  if (shouldUpdate) {
    client.publish(mqtt_announce_topic, "checking for updates...");
    t_httpUpdate_return ret = ESPhttpUpdate.update(update_url, current_version);

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

    shouldUpdate = false;
  }
}
