#ifndef __LED_BLINKER_H__
#define __LED_BLINKER_H__

class LedBlinker {
 public:
  LedBlinker();
  void setup(int output);
  void loop();

  // get/set cycle len in ms
  int cycle() const;
  void cycle(int cycle);
 private:
  int _output;
  int _cycle;
  unsigned long _last_changed;
  int _current_state;
};

#endif // __LED_BLINKER_H__
