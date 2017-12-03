#ifndef Doorway_h
#define Doorway_h

#include "Arduino.h"
#include "EspBug.h"

enum DoorwayState {
  DoorwayStateClear,
  DoorwayState1SensorEntering,
  DoorwayState2Sensor,
  DoorwayState1SensorLeaving
};

enum DoorwayDirection {
  DoorwayDirectionEnter,
  DoorwayDirectionExit
};

class Doorway {
  public:
    Doorway(int sensor1Pin, int sensor2Pin, EspBug* espBug, char* topic);
    void loop();
  private:
    void setDirection(DoorwayDirection dir);
    void doorCrossed();
    const int DEBOUNCE_MICROS = 5000;
    const int MIN_THRESHOLD_MICROS = 75000;
    EspBug* _espBug;
    char* _topic;
    Switch _sensor1;
    Switch _sensor2;
    Switch* _enterSensor;
    Switch* _exitSensor;
    DoorwayState _state;
    DoorwayDirection _direction;
    Timer _crossingTimer;

};

#endif
