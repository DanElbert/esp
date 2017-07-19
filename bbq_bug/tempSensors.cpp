#include "tempSensors.h"

TempSensors::TempSensors(OneWire* oneWire):
  _sensors(oneWire),
  _temp1(0.0),
  _temp2(0.0),
  _conversionTimer(750000)
{
  _sensors.setWaitForConversion(false);
}

void TempSensors::begin() {
  _sensors.begin();

  Serial.print("device count: ");
  Serial.println(_sensors.getDeviceCount(), DEC);

  _sensors.getAddress(_tempAddress1, 0);
  _sensors.getAddress(_tempAddress2, 1);

  _sensors.requestTemperatures();
  _conversionTimer.tick();
}

void TempSensors::update() {

  if (_conversionTimer.tock()) {
    _temp1 = _sensors.getTempF(_tempAddress1);
    _temp2 = _sensors.getTempF(_tempAddress2);

    _sensors.requestTemperatures();
    _conversionTimer.tick();
  }
}

float TempSensors::getTemp1() {
  return _temp1;
}

float TempSensors::getTemp2() {
  return _temp2;
}
