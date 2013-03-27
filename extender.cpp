#include "extender.h"

void Extender::setup(int input, duration_t duration) {
  _input = input;
  _duration = duration;
  _state = _prev_state = 0;

  pinMode(_input, INPUT);
}

void Extender::loop() {
  moment_t now = millis();
  int current_state = digitalRead(_input);

  if (_state == 0 && current_state == 1) {
    _state = 1;
  }
  if (current_state == 1) {
    _activated = now;
  }

  if (_state == 1 && current_state == 0 && now - _activated > _duration) {
    _state = 0;
  }
}

int Extender::stateChange() {
  if (_state != _prev_state) {
    _prev_state = _state;
    return _state;
  }
  return -1;
}
