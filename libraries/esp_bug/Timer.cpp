#include "Timer.h"

Timer::Timer(long usDelay) {
  _usDelay = (unsigned long)usDelay;
}

void Timer::tick() {
  _lastTick = micros();
}

boolean Timer::tock() {
  unsigned long now = micros();
  unsigned long delta = 0;

  if (now >= _lastTick) {
    delta = now - _lastTick;
  } else {
    delta = ~((unsigned long)0) - (_lastTick - now);
  }

  return delta >= _usDelay;
}
