
// #define USE_OLD_HARDWARE


#include <ESP8266WiFi.h>
#include <ESP8266httpUpdate.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <Secrets.h>
#include <EspBug.h>

#ifdef USE_OLD_HARDWARE
#include <Adafruit_HDC1000>
#else
#include <Adafruit_SHT31.h>
#endif


#ifdef USE_OLD_HARDWARE
const char* type = "atmo_bug_legacy";
const char* version = "2.0.0";
#else
const char* type = "atmo_bug";
const char* version = "2.0.5";
#endif

Secrets secrets;
EspBug espBug(type, version, &secrets);
Timer reportTimer(5000000);

char mqtt_temperature_topic[30];
char mqtt_humidity_topic[30];

#ifdef USE_OLD_HARDWARE
Adafruit_HDC1000 sensor = Adafruit_HDC1000();
#else
Adafruit_SHT31 sensor = Adafruit_SHT31();
#endif


void setup() {
  Serial.begin(115200);
  delay(100);

  pinMode(0, OUTPUT);
  digitalWrite(0, HIGH);
  ESPhttpUpdate.setLedPin(0, LOW);
  
  espBug.begin();
  espBug.buildMqttTopic(mqtt_temperature_topic, "temperature");
  espBug.buildMqttTopic(mqtt_humidity_topic, "humidity");


  if (!sensor.begin()) {
    Serial.println("Couldn't find sensor!");
    while (1);
  }
}


void loop() {
  char buffer[8];
  
  espBug.update();
  
  if (reportTimer.tock()) {
    float t = sensor.readTemperature();
    float h = sensor.readHumidity();
    
#ifndef USE_OLD_HARDWARE
    t = (t * 1.8) + 32;
#endif
    
    dtostrf(t, 5, 2, buffer);    
    espBug.mqttPublish(mqtt_temperature_topic, buffer);
    
    dtostrf(h, 5, 2, buffer);    
    espBug.mqttPublish(mqtt_humidity_topic, buffer);
    
    reportTimer.tick();
  }

}
