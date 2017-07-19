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
#include "tempSensors.h"

const char* current_version = "bbq_bug-0.0.1";
char message[50] = "Starting...";

#define OLED_RESET 13
#define ONE_WIRE_BUS 12

Adafruit_SSD1306 display(OLED_RESET);
OneWire oneWire(ONE_WIRE_BUS);
TempSensors tempSensors(&oneWire);

int pinState = 0;
Timer blinkTimer(1000000);
Timer disTimer(250000);

void updateDisplay();
void writeMessage();
int convertBattery(int);
int readAdc();

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

  int rawBatt = readAdc();

  int battmV = convertBattery(rawBatt);
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

  itoa(battmV, message, 10);
  writeMessage();

  display.display();
}

// Given batter reading from adc, conver to real mV (based on voltage divider values)
int convertBattery(int mv) {
  // voltage divider: R1=320K, R2=100K.  4.2V BATT = 1.0V @ ADC
  return (mv * 4200) / 1000;
}

// Returns mV from given ADC input pin
int readAdc() {
  // The raw value ranges from 0 for 0.0V to 1023 for 1.0V
  int raw = analogRead(A0);
  return (raw * 1000) / 1023;
}

void writeMessage() {
  for (int x = 0; message[x]; x++) {
    display.write(message[x]);
  }
}
