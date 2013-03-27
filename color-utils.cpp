#include "color-utils.h"

const float MIN_HUE = 0.0f;
const float MAX_HUE = 360.0f;

void hsv2rgb(float hue, float saturation, float value,
             float *r, float *g, float *b) {
  float c = value * saturation;
  float h2 = hue / 60.0f;
  float x = c * (1 - abs(fmod(h2, 2) - 1));

  double rgb1[][3] = {
    {c, x, 0},
    {x, c, 0},
    {0, c, x},
    {0, x, c},
    {x, 0, c},
    {c, 0, x},
  };
  float m = value - c;

  int idx = int(h2);
  *r = rgb1[idx][0] + m;
  *g = rgb1[idx][1] + m;
  *b = rgb1[idx][2] + m;
}
