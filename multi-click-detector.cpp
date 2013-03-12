// #include "double-click-detector.h"
#include "Arduino.h"

void MultiClickDetector::setup(int port, int max_clicks) {
  _dbc.setup(port, 50);
  _click_num = 0;
  _detected_clicks = 0;
  _max_clicks = max_clicks;
  _fire_after = 0;
}

void MultiClickDetector::loop() {
  unsigned long now = millis();
  _dbc.loop();
  if (_dbc.change() == BTN_INP_DOWN) {
    _fire_after = 0;
    ++ _click_num;
    if (_click_num < _max_clicks) {
      _fire_after = now + MAX_CLICK_DELAY;
    }
    if (_click_num == _max_clicks) {
      _detected_clicks = _max_clicks;
      _click_num = 0;
    }
  }

  if (_click_num && now > _fire_after) {
    _detected_clicks = _click_num;
    _click_num = 0;
  }
}

int MultiClickDetector::getClicks() {
  int result = _detected_clicks;
  _detected_clicks = 0;
  return result;
}
