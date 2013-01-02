// give it a name:
int led = 13;
int inp = 2;
int out_r = 3;
int out_g = 10;
int out_b = 11;

const int MAX_LIGHT = 255;
const unsigned long FADE_DELAY_MS = 15;
const unsigned long MIN_ON_DURATION_MS = 60L * 1000L;

double value;
unsigned long last_millis_hue;
unsigned long last_millis_value;
unsigned long last_movement;

const double HUE_CYCLE_DELAY = 10;
const double HUE_STEP = 0.1;
const double MAX_HUE = 360.0;
double hue;

// the setup routine runs once when you press reset:
void setup() {
  // Serial.begin(9600);
  // Serial.println("Hello fucking world");
  // delay(2000);


  // initialize the digital pin as an output.
  pinMode(led, OUTPUT);
  pinMode(inp, INPUT);
  pinMode(out_r, OUTPUT);
  pinMode(out_g, OUTPUT);
  pinMode(out_b, OUTPUT);

  analogWrite(out_r, 0);
  analogWrite(out_g, 0);
  analogWrite(out_b, 0);

  last_millis_hue = 0;
  last_millis_value = 0;

  hue = 0.0;
  value = 0.0;
}

// the loop routine runs over and over again forever:
// double RED = 0;
void loop() {
  //// This works!
  unsigned long curr_millis = millis();
  boolean lightsOn = true;

  if (digitalRead(inp) == HIGH) {
    last_movement = curr_millis;
  }
  if (curr_millis - last_movement > MIN_ON_DURATION_MS) {
    lightsOn = false;
  }

  if (lightsOn) {
    if (value < 0.02) {
      value = 0.02;
    } else if (value > 0.99) {
      last_millis_value = millis();
      value = 1;
    } else {
      if (curr_millis - last_millis_value > FADE_DELAY_MS) {
        last_millis_value = curr_millis;
        value += 0.01;
      }
    }
  } else { // lightsOff
    if (curr_millis - last_millis_value > FADE_DELAY_MS) {
      if (value < 0.02) {
        value = 0;
      } else {
        last_millis_value = curr_millis;
        value -= 0.01;
      }
    }
  }

  // cycle through colors
  if (curr_millis - last_millis_hue > HUE_CYCLE_DELAY) {
    hue += HUE_STEP;
    hue = fmod(hue, MAX_HUE);
    last_millis_hue = curr_millis;
  }

  double r, g, b;
  hsv2rgb(hue, 0.5, value, r, g, b);

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
double fmod(double x, double y) {
  int quot = int(x / y);
  double rem = x - quot * y;
  return rem;
}

// Hue, Saturation, Value to RGB convertor
// Hue in [0, 360)
// Saturation in [0, 1]
// Value in [0, 1]
//
// returns Red, Green, Blue in [0, 1]
void hsv2rgb(int h, double s, double v, double &r, double &g, double &b) {
  double c = v * s;
  double h2 = h / 60.0;
  double x = c * (1 - abs(fmod(h2, 2) - 1));

  double rgb1[][3] = {
    {c, x, 0},
    {x, c, 0},
    {0, c, x},
    {0, x, c},
    {x, 0, c},
    {c, 0, x},
  };
  double m = v - c;

  int idx = int(h2);
  r = rgb1[idx][0] + m;
  g = rgb1[idx][1] + m;
  b = rgb1[idx][2] + m;

  // Serial.print(h, DEC);
  // Serial.print(" ");
  // Serial.print(r, 2);
  // Serial.print(" ");
  // Serial.print(g, 2);
  // Serial.print(" ");
  // Serial.print(b, 2);
  // Serial.println("");
}
