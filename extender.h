#ifndef __EXTENDER_H__
#define __EXTENDER_H__

#include "utils.h"

class Extender {
 public:
  void setup(int input, duration_t duration);
  void loop();
  int stateChange();
 private:
  enum { EXTEND_STATE = 1 };
  int _input;
  int _state;
  int _prev_state;
  duration_t _duration;
  moment_t _activated;
};

#endif // __EXTENDER_H__
