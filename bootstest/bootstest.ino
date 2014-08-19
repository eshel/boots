#include "NeoPixelParallel.h"
#include "Particle.h"
#include "ColorUtils.h"
#include "Motion.h"
#include "Walker.h"
#include "Animation.h"
#include "Disco.h"
#include "Rain.h"
#include "Led.h"
#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050.h"
#include "Sines.h"
#include "Boom.h"
#include "ModeIndicator.h"

#if defined(__AVR_ATmega32U4__)
#define ARDUINO_IS_PRO_MICRO  1
#else
#define ARDUINO_IS_PRO_MICRO  0
#endif

#if (ARDUINO_IS_PRO_MICRO)
Led led(17);
#else
Led led(7);
#endif


//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
MultiNeoPixel strip = MultiNeoPixel(7, 16, NEO_GRB + NEO_KHZ800);

ParticleSystem particles(strip);

Motion motionSensor;

Disco disco(strip, false);
Walker walker1(strip, false);
Walker walker2(strip, false);
Walker walker3(strip, false);
Walker greenWalker(strip, false);
Rain rain(strip, false);
Sines sines(strip, true);
Boom boom1(strip, true);
Boom boom2(strip, true);
Boom boom3(strip, true);

volatile uint8_t mode = 0;

ModeIndicator modeA(strip, &mode, true);

Animation* s_Animations[] = {
  &sines,
  &disco,
  &rain,  
  &boom1,
  &boom2,
  &boom3,
  &walker1,
  &walker2,
  &walker3,
  &greenWalker
};

static const int s_AnimationsCount = sizeof(s_Animations) / sizeof(Animation*);


// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

// Common main-loop state. consider struct/class.
unsigned long last_update = 0;
unsigned long current_time = 0;

void setup() {
  //Serial.begin(9600);
  randomSeed(analogRead(8));

  Serial.begin(38400);
  Wire.begin();
  led.begin();

  motionSensor.begin();
  bool motionOK = motionSensor.test();
  Serial.println(motionOK ? "Motion init successful" : "Motion init failed");  

  greenWalker.setIsWrapping(false);
  greenWalker.setColorHead(128, 255, 255);
  greenWalker.setColorTrailHue(0);

  for (Animation** a = s_Animations; a != s_Animations + s_AnimationsCount; ++a) {
    (*a)->begin();
  }

  modeA.begin();

  strip.setModeAny();
  strip.clearAll();
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  delay(3000);
  
  last_update = millis();
}

void explodeOne(float maxRadius = 80.0f, uint32_t durationMs = 250) {
  if (!boom1.inProgress()) {
    boom1.set(maxRadius, durationMs);
    boom1.beginExpand();
    return;
  }
  if (!boom2.inProgress()) {
    boom2.set(maxRadius, durationMs);
    boom2.beginExpand();
    return;
  }
  if (!boom3.inProgress()) {
    boom3.set(maxRadius, durationMs);
    boom3.beginExpand();
    return;
  }
}

void onStep() {
  //random_blips(3, 10);
  //strip.setPixelColor(random(0, strip.getSizeX()), random(0, strip.getSizeY()), 255, 255, 255);
  explodeOne((float)random(40, 150));
}



void doMotion() {
  // read raw accel/gyro measurements from device
  static uint32_t lastMotionMs = 0;
  static uint32_t thresholdMs = 150;
  static const int16_t thresholdG = 600;
  int16_t apower = motionSensor.getAPower();

  if (abs(apower-1024) > thresholdG) {
    led.on();
    uint32_t ms = millis();
    if (ms - lastMotionMs > thresholdMs) {
      onStep();
      lastMotionMs = ms;
    }
  } else {
    led.off();
  }

}

static uint32_t frame = 0;

void loop() {
  current_time = millis();
  motionSensor.sample();
  //motionSensor.print();

  doMotion();

  //do_particles();

  //strip.addAll(-25);
  strip.multAll(4, 5);

  if (modeA.shouldDraw()) {
    modeA.draw();
  } else {
    for (Animation** a = s_Animations; a != s_Animations + s_AnimationsCount; ++a) {
      if ((*a)->isActive()) {
        (*a)->draw();
      }
    }
  }

  if (frame % 100 == 0) {
    mode = (mode+1) % 16;
  }

  last_update = current_time;

  strip.show();

  int16_t waitTime = 33 - (int16_t)(millis() - current_time);
  if (waitTime < 10) {
    waitTime = 10;  // Minimal delay - necessary!
  }
  delay(waitTime); // important to have this!  

  frame++;
}


#define INC_MOD(x, lim) (x)++; if ((x) >= (lim)) x=0;

void do_particles() {
  strip.clearAll();
  particles.runFrame(current_time);
  strip.show();
}

/*
void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<768; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) % 768));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<768*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) % 768));
    }
    strip.show();
    delay(wait);
  }
}

*/
