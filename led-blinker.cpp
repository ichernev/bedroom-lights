#include "led-blinker.h"

LedBlinker::LedBlinker() {}

void LedBlinker::setup(int output) {
  _output = output;
  _cycle = 0;
  _last_changed = millis();
  _current_state = LOW;

  pinMode(_output, OUTPUT);
  digitalWrite(_output, _current_state);
}

int LedBlinker::cycle() const { return _cycle; }

void LedBlinker::cycle(int cycle) { _cycle = cycle; }

void LedBlinker::loop() {
  unsigned long now = millis();

  if (_cycle == 0) {
    digitalWrite(_output, LOW);
    _last_changed = now;
    return;
  }

  if (now - _last_changed > _cycle) {
    _current_state = other(_current_state);
    digitalWrite(_output, _current_state);
    _last_changed = now;
  }
}

