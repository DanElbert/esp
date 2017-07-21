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
const char* version = "0.0.5";

Adafruit_SSD1306 oled(OLED_RESET);
OneWire oneWire(ONE_WIRE_BUS);
TempSensors tempSensors(&oneWire);
BatterySensor battery;
Display display(&oled, &tempSensors, &battery);

Secrets secrets;
EspBug espBug(type, version, &secrets);

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

  blinkTimer.tick();
}


void loop() {
  espBug.update();
  tempSensors.update();
  display.update();

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
