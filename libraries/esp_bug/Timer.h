#ifndef TIMER_H
#define TIMER_H

#include "Arduino.h"

class Timer {
  public:
    Timer(long usDelay);
    void tick();
    boolean tock();
  private:
    unsigned long _usDelay;
    unsigned long _lastTick;
};

#endif
