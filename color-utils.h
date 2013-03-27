#ifndef __COLOR_UTILS_H__
#define __COLOR_UTILS_H__

extern const float MIN_HUE;
extern const float MAX_HUE;

// Hue, Saturation, Value to RGB convertor
// Hue in [MIN_HUE, MAX_HUE)
// Saturation in [0, 1]
// Value in [0, 1]
//
// returns Red, Green, Blue in [0, 1]
void hsv2rgb(float hue, float saturation, float value,
             float *r, float *g, float *b);

#endif // __COLOR_UTILS_H__
