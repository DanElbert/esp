#ifndef ESP_BUG_H
#define ESP_BUG_H

#define DEBUG

#ifdef DEBUG
# define DEBUG_PRINT(x) do { Serial.println(x); } while(0)
#else
# define DEBUG_PRINT(x) do { } while(0)
#endif

#include "Arduino.h"
#include "ESP8266WiFi.h"
#include "ESP8266HTTPClient.h"
#include "ESP8266httpUpdate.h"
#include "PubSubClient.h"
#include "Timer.h"
#include "Switch.h"
#include "Secrets.h"

class EspBug {
public:
  EspBug(const char* bug_type, const char* version, Secrets* secrets);
  void insertBugId(char* str);
  void buildMqttTopic(char* topic, char* name);
  void begin();
  void update();
  void mqttCallback(char* topic, byte* payload, unsigned int length);
private:
  WiFiClient _wifi;
  PubSubClient _mqtt;
  Secrets* _secrets;

  const char* _bug_type;
  const char* _version;
  char _id[4];
  char _full_version[30];
  char _name[30];
  char _mqtt_announce_topic[30];
  char _mqtt_ping_topic[30];
  char _mqtt_update_topic[30];
  char _messageBuffer[75];
  bool _shouldUpdate;

  void mqttReconnect();
};

static EspBug* GLOBAL_ESP_BUG;
static void GLOBAL_MQTT_CALLBACK(char* topic, byte* payload, unsigned int length) {
  GLOBAL_ESP_BUG->mqttCallback(topic, payload, length);
}

#endif
