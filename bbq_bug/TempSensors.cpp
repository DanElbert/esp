#include "TempSensors.h"

TempSensors::TempSensors(OneWire* oneWire):
  _sensors(oneWire),
  _one { MAX_CONVERSION_WAIT_MICROS },
  _two { MAX_CONVERSION_WAIT_MICROS }
{
  _sensors.setWaitForConversion(false);
  _one.conversionTimer.tick();
  _two.conversionTimer.tick();
}

void TempSensors::begin() {
  _sensors.begin();

  _sensors.getAddress(_one.address, 0);
  _sensors.getAddress(_two.address, 1);

  _sensors.requestTemperatures();
}

void TempSensors::update() {
  updateTemp(&_one);
  updateTemp(&_two);
}

float TempSensors::getTemp1() {
  return _one.temp;
}

float TempSensors::getTemp2() {
  return _two.temp;
}

void TempSensors::updateTemp(TempSensor* sensor) {
  if (_sensors.isConversionAvailable(sensor->address)) {
    sensor->temp = _sensors.getTempF(sensor->address);
    _sensors.requestTemperaturesByAddress(sensor->address);
    sensor->conversionTimer.tick();
  }

  if (sensor->conversionTimer.tock()) {
    _sensors.requestTemperaturesByAddress(sensor->address);
  }
}
