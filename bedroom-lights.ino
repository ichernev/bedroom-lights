// give it a name:

#undef DEBUG_BUILD

#ifdef DEBUG_BUILD
#define Dbegin Serial.begin
#define Dprint Serial.print
#define Dprintln Serial.println
#define Ddelay  delay
#else
#define Dbegin(...)
#define Dprint(...)
#define Dprintln(...)
#define Ddelay(...)
#endif


#include "led-blinker.h"
#include "multi-click-detector.h"
#include "extender.h"
#include "color-utils.h"
#include "linear-animator.h"

int led = 5;
int motion_sensor = 2;
int btn = 8;
int out_r = 3;
int out_g = 10;
int out_b = 11;

const float SATURATION = 0.5;
const duration_t STAY_ON = 60 * SECOND;
const duration_t COLOR_LOOP_INTERVAL = 10 * SECOND;
const duration_t MANUAL_OFF_HOLD = 10 * SECOND;
const duration_t FADE_OUT_DURATION = 1.5 * SECOND;
const duration_t FADE_IN_DURATION = 1.5 * SECOND;

enum lights_mode_t { NORMAL = 0, MANUAL_OFF = 1, ALWAYS_ON = 2} lights_mode;
moment_t end_manual_off;

MultiClickDetector dblClick;
LedBlinker ledBlinker;
Extender motionSensor;
LinearAnimator hue;
LinearAnimator intensity;

// the setup routine runs once when you press reset:
void setup() {
  Dbegin(9600);
  Dprintln("Iteration 2");
  Ddelay(2000);

  pinMode(out_r, OUTPUT);
  pinMode(out_g, OUTPUT);
  pinMode(out_b, OUTPUT);
  motionSensor.setup(motion_sensor, STAY_ON);
  hue.setup(true, 0.0f);
  hue.reset(MIN_HUE, MAX_HUE, COLOR_LOOP_INTERVAL);

  intensity.setup(false, 0.00f);
  intensity.reset(0, 0, 1);

  analogWrite(out_r, 0);
  analogWrite(out_g, 0);
  analogWrite(out_b, 0);

  dblClick.setup(btn, 2);
  ledBlinker.setup(led);

  lights_mode = NORMAL;
}

// the loop routine runs over and over again forever:
void loop() {
  ledBlinker.loop();
  dblClick.loop();
  motionSensor.loop();

  lights_mode_t new_mode = (lights_mode_t) dblClick.getClicks();
  switch (new_mode) {
    case MANUAL_OFF:
      Dprintln("mode 1");
      lights_mode = new_mode;
      end_manual_off = millis() + MANUAL_OFF_HOLD;
      intensity.reset(1, 0, FADE_OUT_DURATION, true);
      break;
    case ALWAYS_ON:
      Dprintln("mode 2");
      lights_mode = new_mode;
      intensity.reset(0, 1, FADE_IN_DURATION, true);
      break;
  }
  if (lights_mode == MANUAL_OFF && end_manual_off < millis()) {
    Dprintln("mode 0");
    lights_mode = NORMAL;
  }

  if (lights_mode == NORMAL) {
    switch (motionSensor.stateChange()) {
      case 0:
        Dprintln("GOT 0");
        // analogWrite(led, 0);
        intensity.reset(1, 0, FADE_OUT_DURATION, true);
        break;
      case 1:
        Dprintln("GOT 1");
        // analogWrite(led, 100);
        intensity.reset(0, 1, FADE_IN_DURATION, true);
        break;
      default:
        break;
    }
  } else {
    // consume sensor state changes
    int ignored = motionSensor.stateChange();
    if (ignored != -1) {
      Dprint("IGNORE ");
      Dprintln(ignored);
    }
  }

  float r, g, b;
  hsv2rgb(hue.get(), SATURATION, scale_intensity(intensity.get()), &r, &g, &b);

  analogWrite(out_r, scale(r));
  analogWrite(out_g, scale(g));
  analogWrite(out_b, scale(b));

  Ddelay(100);
}

float scale_intensity(float x) {
  return pow(x, 10);
}

int scale(double x) {
  return x * 255.0;
}

// Arduino build system is stupid
#include "color-utils.cpp"
#include "debouncer.cpp"
#include "extender.cpp"
#include "led-blinker.cpp"
#include "linear-animator.cpp"
#include "multi-click-detector.cpp"
#include "utils.cpp"
