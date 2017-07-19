#ifndef BatterySensor_h
#define BatterySensor_h

#include "Arduino.h"

class BatterySensor {
public:
  BatterySensor();

  int readQuartile();
  int readPercent();
  int readBatteryMillivolts();
  int readRawMillivolts();
  int readRawAdc();
};

#endif
