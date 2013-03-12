#ifndef __MULTI_CLICK_DETECTOR_H__
#define __MULTI_CLICK_DETECTOR_H__

#include "Arduino.h"
#include "debouncer.h"

class MultiClickDetector {
 public:
  void setup(int port, int max_clicks);
  void loop();
  int getClicks();
 private:
  Debouncer _dbc;
  int _click_num;
  int _detected_clicks;
  int _max_clicks;
  unsigned long _fire_after;

  // Assume pull-up resistors
  enum {
    BTN_INP_DOWN = 1
  , BTN_INP_UP = 0
  , MAX_CLICK_DELAY = 500
  };
};

#endif // __MULTI_CLICK_DETECTOR_H__
