#ifndef BatterySensor_h
#define BatterySensor_h

#include "Arduino.h"
#include "RingArray.h"

class BatterySensor {
public:
  BatterySensor();
  void update();

  int readPercent();
  int readBatteryMillivolts();
  int readRawMillivolts();
  int readRawAdc();

private:
  RingArray _readingBuffer;
};

#endif
