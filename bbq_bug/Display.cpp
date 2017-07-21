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
  _oled->setTextSize(3);
  _oled->setCursor(0,0);

  float temp1 = _temps->getTemp1();
  float temp2 = _temps->getTemp2();

  writeFloat(temp1, 0);
  _oled->write(' ');
  writeFloat(temp2, 0);

  _oled->write('\n');
  _oled->setTextSize(1);
  writeInt(_battery->readBatteryMillivolts());
  _oled->write(' ');
  writeInt(WiFi.RSSI());

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

  // battery is 20px wide; display is 128x32
  int originX = 107;
  int originY = 0;
  int width = 20;
  int height = 16;
  int space = 2;
  int barCount = 4;
  int barWidth = 2;

  _oled->drawRect(originX, originY, width, height, WHITE);
  for (int i = 0; i < bars; i++) {
    int x = originX + 1 + space + (i * (barWidth + space));
    int y = originY + 1 + space;
    for (int j = 0; j < barWidth; j++) {
      _oled->drawLine(x + j, y, x + j, y + 9, WHITE);
    }
  }

}

void Display::drawWiFi() {
  int32_t rssi = WiFi.RSSI();
  int bars;

  if (rssi > -60) {
    bars = 4;
  } else if (rssi > -70) {
    bars = 3;
  } else if (rssi > -80) {
    bars = 2;
  } else {
    bars = 1;
  }

  int barCount = 4;
  int barWidth = 3;
  int space = 2;
  int originX = 127 - ((barCount * barWidth) + (space * (barCount - 1)));
  int originY = 16;
  int minHeight = 3;
  int maxHeight = 15;
  int barDiff = (maxHeight - minHeight) / barCount;

  for (int b=0; b < bars; b++) {
    int barHeight = minHeight + (b * barDiff);
    int rx = originX + (b * (barWidth + space));
    int ry = originY + (maxHeight - barHeight);
    for (int i = 0; i < barWidth; i++) {
      _oled->drawLine(rx + i, ry, rx + i, ry + barHeight, WHITE);
    }
    //_oled->fillRect(originX + (b * 3), originY + maxBarHeight - 1 - (b*3), 2, b * 3, WHITE);
    //display.fillRect(59 + (b*5),33 - (b*5),3,b*5,WHITE);
  }

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
