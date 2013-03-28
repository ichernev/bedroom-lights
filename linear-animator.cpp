#include "linear-animator.h"

void LinearAnimator::setup(bool loop, float cut_ends) {
  _loop = loop;
  _cut_ends = cut_ends;
}

void LinearAnimator::reset(float from, float to, duration_t duration, bool keep_current) {
  if (keep_current) {
    _start = millis() - (get() - from) / (to - from) * duration;
  } else {
    _start = millis();
  }
  _from = from;
  _to = to;
  _duration = duration;
}

float LinearAnimator::get() const {
  moment_t now = millis();
  float coef = getCoef();

  return _from + coef * (_to - _from);
}

float LinearAnimator::getCoef() const {
  float coef = float(millis() - _start) / float(_duration);
  if (_loop) {
    coef = fmod(coef, 1.0);
  }
  if (coef < 0 + _cut_ends) coef = 0.0f;
  if (coef > 1 - _cut_ends) coef = 1.0f;
  return coef;
}
