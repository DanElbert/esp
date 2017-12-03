#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
#include <PubSubClient.h>
#include <Secrets.h>
#include <EspBug.h>
#include "Doorway.h"

// #define DEBUG

const char* type = "door_bug";
const char* version = "0.1.0";

Secrets secrets;
EspBug espBug(type, version, &secrets);


char mqtt_traversal_topic[30];

Doorway door(13, 14, &espBug, mqtt_traversal_topic);

void setup() {
  Serial.begin(115200);
  delay(100);

  pinMode(0, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, INPUT_PULLUP);
  pinMode(14, INPUT_PULLUP);

  espBug.begin();
  espBug.buildMqttTopic(mqtt_traversal_topic, "traversal");

  analogWriteFreq(38000);
  analogWriteRange(127);
  analogWrite(12, 63);
}

void loop() {
  espBug.update();
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
}
