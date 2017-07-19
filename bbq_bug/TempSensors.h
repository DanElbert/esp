#ifndef TempSensors_h
#define TempSensors_h

#include "Arduino.h"
#include "OneWire.h"
#include "DallasTemperature.h"
#include "timer.h"

struct TempSensor {
  TempSensor(int conversionWait) :
    address({}),
    temp(0.0),
    conversionTimer(conversionWait)
  {}

  DeviceAddress address;
  float temp;
  Timer conversionTimer;
};

class TempSensors {
public:
  TempSensors(OneWire* oneWire);
  void begin();
  void update();
  float getTemp1();
  float getTemp2();
private:
  const static int MAX_CONVERSION_WAIT_MICROS = 1000000;
  DallasTemperature _sensors;
  TempSensor _one;
  TempSensor _two;
  void updateTemp(TempSensor* sensor);
};

#endif
