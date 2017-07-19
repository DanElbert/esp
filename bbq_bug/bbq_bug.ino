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
#include <secrets.h>
#include <esp_bug.h>
#include "TempSensors.h"
#include "BatterySensor.h"

const char* current_version = "bbq_bug-0.0.1";
char message[50] = "Starting...";

#define OLED_RESET 13
#define ONE_WIRE_BUS 12

Adafruit_SSD1306 display(OLED_RESET);
OneWire oneWire(ONE_WIRE_BUS);
TempSensors tempSensors(&oneWire);
BatterySensor battery;

int pinState = 0;
Timer blinkTimer(1000000);
Timer disTimer(250000);

void updateDisplay();
void writeMessage();

void setup() {
  Serial.begin(115200);
  delay(100);

  pinMode(0, OUTPUT);
  digitalWrite(0, LOW);

  tempSensors.begin();

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // initialize with the I2C addr 0x3C (for the 128x32)
  display.clearDisplay();

  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,0);

  writeMessage();
  display.display();

  blinkTimer.tick();
  disTimer.tick();
}


void loop() {
  tempSensors.update();

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

  if (disTimer.tock()) {
    updateDisplay();
    disTimer.tick();
  }
}

void updateDisplay() {
  display.clearDisplay();
  display.setCursor(0,0);

  float temp1 = tempSensors.getTemp1();
  float temp2 = tempSensors.getTemp2();

  itoa(temp1, message, 10);
  //dtostrf(temp1, 4, 1, message);
  writeMessage();

  display.write(' ');

  itoa(temp2, message, 10);
  //dtostrf(temp2, 4, 1, message);
  writeMessage();

  display.write('\n');

  itoa(battery.readBatteryMillivolts(), message, 10);
  writeMessage();

  display.write(' ');
  itoa(battery.readPercent(), message, 10);
  writeMessage();

  display.display();
}

void writeMessage() {
  for (int x = 0; message[x]; x++) {
    display.write(message[x]);
  }
}
