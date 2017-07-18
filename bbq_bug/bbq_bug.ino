#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_ADS1015.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <PubSubClient.h>
#include <secrets.h>
#include <esp_bug.h>

const char* current_version = "bbq_bug-0.0.1";
char message[50] = "Starting...";

#define OLED_RESET 13
Adafruit_SSD1306 display(OLED_RESET);

Adafruit_ADS1015 ads;

int pinState = 0;
Timer blinkTimer(1000000);
Timer disTimer(250000);

void updateDisplay();
void writeMessage();
int convertTemp(int);
int convertBattery(int);
int readAdc(int);

void setup() {
  Serial.begin(115200);
  delay(100);

  ads.setGain(GAIN_ONE);
  ads.setSPS(ADS1015_DR_250SPS);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // initialize with the I2C addr 0x3C (for the 128x32)
  display.clearDisplay();

  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,0);

  writeMessage();
  display.display();

  pinMode(0, OUTPUT);
  digitalWrite(0, LOW);
  blinkTimer.tick();
  disTimer.tick();
  ads.begin();
}


void loop() {
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

  int rawTemp1 = readAdc(0);
  int rawTemp2 = readAdc(1);
  int rawBatt = readAdc(2);

  int battmV = convertBattery(rawBatt);
  int temp1 = convertTemp(rawTemp1);
  int temp2 = convertTemp(rawTemp2);
  
  itoa(rawTemp1, message, 10);
  writeMessage();

  display.write(' ');

  itoa(temp1, message, 10);
  writeMessage();

  display.write('\n');

  itoa(rawTemp2, message, 10);
  writeMessage();

  display.write(' ');

  itoa(temp2, message, 10);
  writeMessage();
  
  display.display();
}

// Given batter reading from adc, conver to real mV (based on voltage divider values)
int convertBattery(int mv) {
  // voltage divider: R1=100K, R2=320K.  4.2V BATT = 3.20V @ ADC
  return (mv * 4200) / 3200;
}

int convertTemp(int mv) {
  // Each 5mV is 1 C, with an offset of 1.25v
  // Experimentally, the mV value is always ~15mV low
  int mVcorrection = 0;
  
  int deg_c = (((mv + mVcorrection) * 10) - 12500) / 5;
  return (((deg_c * 9) / 5) + 320) / 10;
}

// Returns mV from given ADC input pin
int readAdc(int input) {
  int raw = ads.readADC_SingleEnded(input);
  // gain is 1, so 1 bit = 2 mV
  return raw * 2;
}

void writeMessage() {
  for (int x = 0; message[x]; x++) {
    display.write(message[x]);
  }
}

