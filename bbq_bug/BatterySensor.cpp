#include "BatterySensor.h"

BatterySensor::BatterySensor() {

}

int BatterySensor::readQuartile() {
  return 0;
}

int BatterySensor::readPercent() {
  // 100% is 4.2v, LOW is 3.3v
  int shift = -3300;
  int max = 4200 + shift;
  return ((readBatteryMillivolts() + shift) * 100) / max;
}

int BatterySensor::readBatteryMillivolts() {
  // voltage divider: R1=320K, R2=100K.  4.2V BATT = 1.0V @ ADC
  return (readRawMillivolts() * 4200) / 1000;
}

int BatterySensor::readRawMillivolts() {
  // The raw value ranges from 0 for 0.0V to 1023 for 1.0V
  return (readRawAdc() * 1000) / 1023;
}

int BatterySensor::readRawAdc() {
  return analogRead(A0);
}
