#include "debouncer.h"

void Debouncer::setup(int port, unsigned long stability_delay) {
  _port = port;
  _started = 0;
  _change = -1;
  _stability_delay = stability_delay;

  pinMode(_port, INPUT);
  // pull up resistors
  digitalWrite(_port, HIGH);
  _state = digitalRead(_port);
}

void Debouncer::loop() {
  int crnt_state = digitalRead(_port);
  unsigned long now = millis();

  if (_started && now - _started > _stability_delay) {
    _started = 0;
    if (crnt_state != _state) {
      _change = crnt_state;
      _state = crnt_state;
    }
  }

  if (!_started && crnt_state != _state) {
    _started = now;
  }
}

int Debouncer::change() {
  int result = _change;
  _change = -1;
  return result;
}
