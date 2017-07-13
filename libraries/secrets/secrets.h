#ifndef secrets_h
#define secrets_h

#include "Arduino.h"

struct BugIdentifier {
  int chipId;
  const char bugId[4];
};

class Secrets {
public:
  const char* getBugId();
  const char* getUpdateUrl();
  const char* getWifiSsid();
  const char* getWifiPassword();
  const char* getMqttServer();
  const char* getMqttUser();
  const char* getMqttPassword();
private:

  static const BugIdentifier BUG_IDS[];
  static const int BUG_ID_COUNT;
  static const char* UPDATE_URL;
  static const char* WIFI_SSID;
  static const char* WIFI_PASSWORD;
  static const char* MQTT_SERVER;
  static const int MQTT_PORT;
  static const char* MQTT_USER;
  static const char* MQTT_PASSWORD;
  static const char* DEFAULT_BUG_ID;
};

#endif
