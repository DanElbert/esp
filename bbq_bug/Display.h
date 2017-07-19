#ifndef Display_h
#define Display_h

#include "Arduino.h"
#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"
#include "TempSensors.h"
#include "BatterySensor.h"
#include "timer.h"

class Display {
public:
  Display(Adafruit_SSD1306* oled, TempSensors* temps, BatterySensor* battery);
  void begin();
  void update();
private:
  static const int REFRESH_DELAY = 500000;
  Adafruit_SSD1306* _oled;
  TempSensors* _temps;
  BatterySensor* _battery;
  Timer _refreshTimer;
  char _buffer[100];

  void writeToDisplay();
  void writeBuffer();
  void writeBuffer(char* buffer);
  void writeFloat(float value, int precision);
  void writeInt(int value);
};

#endif
