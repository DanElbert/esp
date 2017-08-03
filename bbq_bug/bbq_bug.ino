#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
#include <SPI.h>
#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <PubSubClient.h>
#include <Secrets.h>
#include <EspBug.h>
#include "TempSensors.h"
#include "BatterySensor.h"
#include "Display.h"

#define OLED_RESET 13
#define ONE_WIRE_BUS 12

const char* type = "bbq_bug";
const char* version = "0.0.8";

Adafruit_SSD1306 oled(OLED_RESET);
OneWire oneWire(ONE_WIRE_BUS);
TempSensors tempSensors(&oneWire);
BatterySensor battery;
Display display(&oled, &tempSensors, &battery);

Secrets secrets;
EspBug espBug(type, version, &secrets);

Timer sendTimer(2000000);
char sendTemp1Topic[30];
char sendTemp2Topic[30];
char sendBatTopic[30];
char sendMessage[30];

int pinState = 0;
Timer blinkTimer(1000000);

void setup() {
  Serial.begin(115200);
  delay(100);

  pinMode(0, OUTPUT);
  digitalWrite(0, LOW);

  espBug.begin();
  tempSensors.begin();
  display.begin();

  espBug.buildMqttTopic(sendTemp1Topic, "temp1");
  espBug.buildMqttTopic(sendTemp2Topic, "temp2");
  espBug.buildMqttTopic(sendBatTopic, "battery");

  blinkTimer.tick();
  sendTimer.tick();
}


void loop() {
  espBug.update();
  battery.update();
  tempSensors.update();
  display.update();

  if (sendTimer.tock()) {
    //itoa(tempSensors.getTemp1(), sendMessage, 10);
    dtostrf(tempSensors.getTemp1(), 2, 2, sendMessage);
    espBug.mqttPublish(sendTemp1Topic, sendMessage);

    //itoa(tempSensors.getTemp2(), sendMessage, 10);
    dtostrf(tempSensors.getTemp2(), 2, 2, sendMessage);
    espBug.mqttPublish(sendTemp2Topic, sendMessage);

    itoa(battery.readBatteryMillivolts(), sendMessage, 10);
    espBug.mqttPublish(sendBatTopic, sendMessage);

    sendTimer.tick();
  }

  if (blinkTimer.tock()) {
    if (pinState == 0) {
      digitalWrite(0, HIGH);
      pinState = 1;
    } else {
      digitalWrite(0, LOW);
      pinState = 0;
    }
    blinkTimer.tick();
  }
}
