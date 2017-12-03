#include "Doorway.h"

Doorway::Doorway(int sensor1Pin, int sensor2Pin, EspBug* espBug, char* topic):
  _sensor1(sensor1Pin, DEBOUNCE_MICROS),
  _sensor2(sensor2Pin, DEBOUNCE_MICROS),
  _state(DoorwayStateClear),
  _direction(DoorwayDirectionEnter),
  _enterSensor(&_sensor1),
  _exitSensor(&_sensor2),
  _espBug(espBug),
  _topic(topic),
  _crossingTimer(MIN_THRESHOLD_MICROS)
{

}

void Doorway::setDirection(DoorwayDirection dir) {
  _direction = dir;
  _crossingTimer.tick();
  if (_direction == DoorwayDirectionEnter) {
    _enterSensor = &_sensor1;
    _exitSensor = &_sensor2;
  } else {
    _enterSensor = &_sensor2;
    _exitSensor = &_sensor1;
  }
}

void Doorway::doorCrossed() {
  if (_crossingTimer.tock()) {
    if (_direction == DoorwayDirectionEnter) {
      _espBug->mqttPublish(_topic, "-1");
    } else {
      _espBug->mqttPublish(_topic, "1");
    }
  }
}

void Doorway::loop() {
  _sensor1.update();
  _sensor2.update();

  switch(_state) {

    case DoorwayStateClear:
      if (_sensor1.read() && _sensor2.read()) {
        // Weird case where both are tripped at the same time
        setDirection(DoorwayDirectionEnter);
        _state = DoorwayState2Sensor;

      } else if (_sensor1.read()) {
        // begin entering
        setDirection(DoorwayDirectionEnter);
        _state = DoorwayState1SensorEntering;

      } else if (_sensor2.read()) {
        // begin exiting
        setDirection(DoorwayDirectionExit);
        _state = DoorwayState1SensorEntering;
      }
      break;


    case DoorwayState1SensorEntering:
      if (_enterSensor->read() && _exitSensor->read()) {
        _state = DoorwayState2Sensor;
      } else if (_enterSensor->read()) {
        // do nothing
      } else if (_exitSensor->read()) {
        // 2 sensor state was skipped
        _state = DoorwayState1SensorLeaving;
      } else {
        // Left without passing through door
        _state = DoorwayStateClear;
      }
      break;


    case DoorwayState2Sensor:
      if (_enterSensor->read() && _exitSensor->read()) {
        // Do nothing
      } else if (_enterSensor->read()) {
        // returning the way they came
        _state = DoorwayState1SensorEntering;
      } else if (_exitSensor->read()) {
        // Continuing to leave
        _state = DoorwayState1SensorLeaving;
      } else {
        // Both sensors deactivated at the same time... guess that they passed through
        doorCrossed();
        _state = DoorwayStateClear;
      }
      break;


    case DoorwayState1SensorLeaving:
      if (_enterSensor->read() && _exitSensor->read()) {
        _state = DoorwayState2Sensor;
      } else if (_enterSensor->read()) {
        // returning the way they came (rapidly)
        _state = DoorwayState1SensorEntering;
      } else if (_exitSensor->read()) {
        // do nothing
      } else {
        // Passed through doorway
        doorCrossed();
        _state = DoorwayStateClear;
      }
      break;

  }
}
