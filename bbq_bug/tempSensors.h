#ifndef Doorway_h
#define Doorway_h

#include "Arduino.h"
#include "OneWire.h"
#include "DallasTemperature.h"
#include "timer.h"

class TempSensors {
public:
  TempSensors(OneWire* oneWire);
  void begin();
  void update();
  float getTemp1();
  float getTemp2();
private:
  DallasTemperature _sensors;
  DeviceAddress _tempAddress1;
  DeviceAddress _tempAddress2;
  float _temp1;
  float _temp2;
  Timer _conversionTimer;
};

#endif
