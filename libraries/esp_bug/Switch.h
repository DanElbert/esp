#ifndef SWITCH_H
#define SWITCH_H

#include "Timer.h"

class Switch {
  public:
    Switch(int pin, long usDelay);
    void update();
    boolean read();
  private:
    int _pin;
    boolean _waiting;
    int _state;
    Timer _timer;
};

#endif
