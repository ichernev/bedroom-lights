// give it a name:

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
const duration_t FADE_DURATION = 2 * SECOND;

enum lights_mode_t { NORMAL = 0, MANUAL_OFF = 1, ALWAYS_ON = 2} lights_mode;
moment_t end_manual_off;

// float dir;
// int mode = 0;

// const int MAX_LIGHT = 255;
// const unsigned long FADE_DELAY_MS = 15;
// const unsigned long MIN_ON_DURATION_MS = 60L * 1000L;

// double value;
// unsigned long last_millis_hue;
// unsigned long last_millis_value;
// unsigned long last_movement;

// const double HUE_CYCLE_DELAY = 10;
// const double HUE_STEP = 0.1;
// const double MAX_HUE = 360.0;
// double hue;

// void assert(int val) {
//   return;
//   // while (!val) {}
// }

// int other(int x) {
//   assert(LOW == 0);
//   assert(HIGH == 1);
//   assert(INPUT == 0);
//   assert(OUTPUT == 1);

//   return 1 - x;
// }

MultiClickDetector dblClick;
LedBlinker ledBlinker;
Extender motionSensor;
LinearAnimator hue;
LinearAnimator intensity;

// the setup routine runs once when you press reset:
void setup() {
  // Serial.begin(9600);
  // Serial.println("Hello fucking world");
  // delay(2000);


  // pinMode(13, OUTPUT);
  // digitalWrite(13, LOW);
  // initialize the digital pin as an output.
  // pinMode(led, OUTPUT);
  // pinMode(inp, INPUT);
  pinMode(out_r, OUTPUT);
  pinMode(out_g, OUTPUT);
  pinMode(out_b, OUTPUT);
  motionSensor.setup(motion_sensor, STAY_ON);
  hue.setup(true, 0.0f);
  hue.reset(MIN_HUE, MAX_HUE, COLOR_LOOP_INTERVAL);

  // dir = 1.0;
  intensity.setup(false, 0.05f);
  intensity.reset(0, 0, 1);

  // pinMode(led, OUTPUT);
  // digitalWrite(led, 0);
  // ledBlinker.setup(led);
  // ledBlinker.cycle(500);

  analogWrite(out_r, 0);
  analogWrite(out_g, 0);
  analogWrite(out_b, 0);

  // last_millis_hue = 0;
  // last_millis_value = 0;

  // hue = 0.0;
  // value = 0.0;

  dblClick.setup(btn, 2);
  ledBlinker.setup(led);

  lights_mode = NORMAL;
}

// the loop routine runs over and over again forever:
// double RED = 0;
void loop() {
  //// This works!
  // unsigned long curr_millis = millis();
  // boolean lightsOn = true;

  // if (digitalRead(inp) == HIGH) {
  //   last_movement = curr_millis;
  // }
  // if (curr_millis - last_movement > MIN_ON_DURATION_MS) {
  //   lightsOn = false;
  // }

  // if (lightsOn) {
  //   if (value < 0.02) {
  //     value = 0.02;
  //   } else if (value > 0.99) {
  //     last_millis_value = millis();
  //     value = 1;
  //   } else {
  //     if (curr_millis - last_millis_value > FADE_DELAY_MS) {
  //       last_millis_value = curr_millis;
  //       value += 0.01;
  //     }
  //   }
  // } else { // lightsOff
  //   if (curr_millis - last_millis_value > FADE_DELAY_MS) {
  //     if (value < 0.02) {
  //       value = 0;
  //     } else {
  //       last_millis_value = curr_millis;
  //       value -= 0.01;
  //     }
  //   }
  // }

  // // cycle through colors
  // if (curr_millis - last_millis_hue > HUE_CYCLE_DELAY) {
  //   hue += HUE_STEP;
  //   hue = fmod(hue, MAX_HUE);
  //   last_millis_hue = curr_millis;
  // }

  // double r, g, b;
  // hsv2rgb(hue, 0.5, value, r, g, b);

  // analogWrite(out_r, scale(r));
  // analogWrite(out_g, scale(g));
  // analogWrite(out_b, scale(b));

  //////////////////////////////
  ///////////////////////////////
  // dblClick.loop();
  // int clicks = dblClick.getClicks();
  // int new_mode = -1;
  // if (clicks == 1) {
  //   ledBlinker.cycle(500);
  //   new_mode = 1;
  // } else if (clicks == 2) {
  //   ledBlinker.cycle(1000);
  //   new_mode = 2;
  // }
  // if (new_mode != -1) {
  //   if (new_mode == mode) {
  //     ledBlinker.cycle(0);
  //     mode = 0;
  //   } else {
  //     mode = new_mode;
  //   }
  // }

  ledBlinker.loop();
  dblClick.loop();
  motionSensor.loop();
  lights_mode_t new_mode = (lights_mode_t) dblClick.getClicks();
  switch (new_mode) {
    case MANUAL_OFF:
      lights_mode = new_mode;
      end_manual_off = millis() + MANUAL_OFF_HOLD;
      intensity.reset(1, 0, FADE_DURATION, true);
      break;
    case ALWAYS_ON:
      lights_mode = new_mode;
      intensity.reset(0, 1, FADE_DURATION, true);
      break;
  }
  if (lights_mode == MANUAL_OFF && end_manual_off < millis()) {
    lights_mode = NORMAL;
  }

  if (lights_mode == NORMAL) {
    switch (motionSensor.stateChange()) {
      case 0:
        // analogWrite(led, 0);
        intensity.reset(1, 0, FADE_DURATION);
        break;
      case 1:
        // analogWrite(led, 100);
        intensity.reset(0, 1, FADE_DURATION);
        break;
      default:
        break;
    }
  } else {
    // consume sensor state changes
    motionSensor.stateChange();
  }

  switch (lights_mode) {
    case MANUAL_OFF:
      ledBlinker.cycle(100);
      break;
    case ALWAYS_ON:
      ledBlinker.cycle(500);
      break;
    case NORMAL:
      ledBlinker.cycle(1000);
      break;
  }

  // if (dir == intensity.get()) {
  //   dir = 1.0f - dir;
  //   intensity.reset(1.0f - dir, dir, 2000);
  // }

  float r, g, b;
  hsv2rgb(hue.get(), SATURATION, intensity.get(), &r, &g, &b);

  analogWrite(out_r, scale(r));
  analogWrite(out_g, scale(g));
  analogWrite(out_b, scale(b));
}

int scale(double x) {
  //return (x * x * 0.97 + 0.03) * 255.0;
  //return (2*x - 1) * 255.0;
  //return x*x*x * 255.0;
  return x * 255.0;
}

// floating point remainder of x / y
// double fmod(double x, double y) {
//   int quot = int(x / y);
//   double rem = x - quot * y;
//   return rem;
// }

// Hue, Saturation, Value to RGB convertor
// Hue in [0, 360)
// Saturation in [0, 1]
// Value in [0, 1]
//
// returns Red, Green, Blue in [0, 1]
// void hsv2rgb(int h, double s, double v, double &r, double &g, double &b) {
//   double c = v * s;
//   double h2 = h / 60.0;
//   double x = c * (1 - abs(fmod(h2, 2) - 1));

//   double rgb1[][3] = {
//     {c, x, 0},
//     {x, c, 0},
//     {0, c, x},
//     {0, x, c},
//     {x, 0, c},
//     {c, 0, x},
//   };
//   double m = v - c;

//   int idx = int(h2);
//   r = rgb1[idx][0] + m;
//   g = rgb1[idx][1] + m;
//   b = rgb1[idx][2] + m;

//   // Serial.print(h, DEC);
//   // Serial.print(" ");
//   // Serial.print(r, 2);
//   // Serial.print(" ");
//   // Serial.print(g, 2);
//   // Serial.print(" ");
//   // Serial.print(b, 2);
//   // Serial.println("");
// }

// Arduino build system is stupid
#include "color-utils.cpp"
#include "debouncer.cpp"
#include "extender.cpp"
#include "led-blinker.cpp"
#include "linear-animator.cpp"
#include "multi-click-detector.cpp"
#include "utils.cpp"
