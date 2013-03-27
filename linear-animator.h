#ifndef __LINEAR_ANIMATOR_H__
#define __LINEAR_ANIMATOR_H__

#include "utils.h"

class LinearAnimator {
 public:
  void setup(bool loop = false, float cut_ends = 0.0f);
  void reset(float from, float to, duration_t duration, bool keep_current = false);
  float get() const;
 private:
  float getCoef() const;
  float _from;
  float _to;
  duration_t _duration;
  moment_t _start;
  bool _loop;
  float _cut_ends;
};

#endif // __LINEAR_ANIMATOR_H__
