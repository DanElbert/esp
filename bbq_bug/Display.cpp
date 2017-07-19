#include "Display.h"

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

  _oled->setTextSize(2);
  _oled->setTextColor(WHITE);
  _oled->setCursor(0,0);
}

void Display::update() {
  if (_refreshTimer.tock()) {
    writeToDisplay();
    _refreshTimer.tick();
  }
}

void Display::writeToDisplay() {
  _oled->clearDisplay();
  _oled->setCursor(0,0);

  float temp1 = _temps->getTemp1();
  float temp2 = _temps->getTemp2();

  writeFloat(temp1, 0);
  _oled->write(' ');
  writeFloat(temp2, 0);

  _oled->write('\n');

  writeInt(_battery->readBatteryMillivolts());
  _oled->write(' ');
  writeInt(_battery->readPercent());

  _oled->display();
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
