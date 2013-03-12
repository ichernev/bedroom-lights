#ifndef __DEBOUNCER_H__
#define __DEBOUNCER_H__

#include "Arduino.h"

class Debouncer {
 public:
  void setup(int port, unsigned long stability_delay = 50);
  void loop();
  int change(); // -1 -- no change. 0 or 1 -- changed to that
 private:
  int _port;
  unsigned long _stability_delay;

  unsigned long _started;
  int _state;
  int _change;
};

#endif // __DEBOUNCER_H__
