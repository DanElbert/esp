#include "Switch.h"

Switch::Switch(int pin, long usDelay) : _timer(usDelay), _pin(pin), _state(LOW), _waiting(false) {
}

void Switch::update() {
  int currentValue = digitalRead(_pin);

  if (currentValue != _state && !_waiting) {
    _timer.tick();
    _waiting = true;
  } else if (currentValue != _state && _waiting && _timer.tock()) {
    _state = currentValue;
    _waiting = false;
  } else if (currentValue == _state) {
    _waiting = false;
  }
}

boolean Switch::read() {
  return _state == HIGH;
}
