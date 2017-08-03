#include "Display.h"

// OLED Manipulation Logic
// screen is 128x32
Display::Display(Adafruit_SSD1306* oled, TempSensors* temps, BatterySensor* battery):
  _oled(oled),
  _temps(temps),
  _battery(battery),
  _buffer({}),
  _refreshTimer(REFRESH_DELAY)
{
  _refreshTimer.tick();
}

void Display::begin() {
  _oled->begin(SSD1306_SWITCHCAPVCC, 0x3C); // initialize with the I2C addr 0x3C (for the 128x32)
  _oled->clearDisplay();
  _oled->setTextColor(WHITE);
}

void Display::update() {
  if (_refreshTimer.tock()) {
    writeToDisplay();
    _refreshTimer.tick();
  }
}

void Display::writeToDisplay() {
  _oled->clearDisplay();
  _oled->setTextSize(3);

  float temp1 = _temps->getTemp1();
  float temp2 = _temps->getTemp2();

  _oled->setCursor(0,8);
  if (temp1 > -195) {
    writeFloat(temp1, 0);
  }  else {
    writeBuffer("N/A");
  }

  _oled->setCursor(72,8);
  if (temp2 > -195) {
    writeFloat(temp2, 0);
  }  else {
    writeBuffer("N/A");
  }

  // _oled->setTextSize(1);
  // _oled->setCursor(0,0);
  // writeInt(_battery->readBatteryMillivolts());

  drawBattery();
  drawWiFi();

  _oled->display();
}

void Display::drawBattery() {

  int battMv = _battery->readBatteryMillivolts();
  int bars = 0;

  if (battMv > 3900) {
    bars = 4;
  } else if (battMv > 3700) {
    bars = 3;
  } else if (battMv > 3500) {
    bars = 2;
  } else if (battMv > 3400) {
    bars = 1;
  }

  int originX = 114;
  int originY = 0;
  int width = 13 - 2;
  int height = 8 - 2;

  _oled->drawRect(originX, originY, width, height + 1, WHITE);
  _oled->drawLine(originX + width, originY + 2, originX + width, originY + 4, WHITE);
  for (int i = 0; i < bars; i++) {
    _oled->drawLine(originX + 2 + (i * 2), originY + 2, originX + 2 + (i * 2), originY + 4, WHITE);
  }
}

void Display::drawWiFi() {
  int32_t rssi = WiFi.RSSI();
  int bars;

  // >-60=good; <-80=bad
  // if (rssi > -60) {
  //   bars = 4;
  // } else if (rssi > -70) {
  //   bars = 3;
  // } else if (rssi > -80) {
  //   bars = 2;
  // } else {
  //   bars = 1;
  // }

  bars = constrain(map(rssi, -90, -50, 0, 10), 0, 10);

  int originX = 100;
  int originY = 0;
  int width = 10;
  int height = 6;

  _oled->drawTriangle(
      originX + width, originY,
      originX + width, originY + height,
      originX, originY + height,
      WHITE);

  int yOffset = height - ((height * bars) / width);
  _oled->fillTriangle(
      originX + bars, originY + yOffset,
      originX + bars, originY + height,
      originX, originY + height,
      WHITE);
}

void Display::writeFloat(float value, int precision) {
  if (precision == 0) {
    itoa(value, _buffer, 10);
  } else {
    dtostrf(value, 2, precision, _buffer);
  }
  writeBuffer();
}

void Display::writeInt(int value) {
  itoa(value, _buffer, 10);
  writeBuffer();
}

void Display::writeBuffer() {
  writeBuffer(_buffer);
}

void Display::writeBuffer(char* buff) {
  for (int x = 0; buff[x]; x++) {
    _oled->write(buff[x]);
  }
}
