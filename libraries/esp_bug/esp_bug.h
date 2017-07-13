#ifndef ESP_BUG_H
#define ESP_BUG_H

#include "Arduino.h"
#include "timer.h"
#include "switch.h"
#include "secrets.h"

class EspBug {
public:
  EspBug(const char* bug_type, const char* version, Secrets* secrets);
private:

};

#endif
