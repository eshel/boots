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


//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
MultiNeoPixel strip = MultiNeoPixel(7, 16, NEO_GRB + NEO_KHZ800);

ParticleSystem particles(strip);

Motion motionSensor;
Led led(17);

Disco disco(strip, false);
Walker walker1(strip, true);
Walker walker2(strip, true);
Walker walker3(strip, true);
Rain rain(strip, false);
Sines sines(strip, true);

Animation* s_Animations[] = {
  &disco,
  &walker,
  &rain,
  &sines
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

  for (Animation** a = s_Animations; a != s_Animations + s_AnimationsCount; ++a) {
    (*a)->begin();
  }

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  strip.setModeAny();

  delay(3000);
  
  last_update = millis();
}


void onStep() {
  //random_blips(3, 10);
  //strip.setPixelColor(random(0, strip.getSizeX()), random(0, strip.getSizeY()), 255, 255, 255);
}



void doMotion() {
  // read raw accel/gyro measurements from devic
  int16_t apower = motionSensor.getAPower();

  if (abs(apower-1024) > 300) {
    led.on();
    onStep();
  } else {
    led.off();
  }

}


void loop() {
  motionSensor.sample();
  motionSensor.print();

  doMotion();

  current_time = millis();
  //do_particles();

  //strip.addAll(-25);
  strip.multAll(4, 5);

  for (Animation** a = s_Animations; a != s_Animations + s_AnimationsCount; ++a) {
    if ((*a)->isActive()) {
      (*a)->draw();
    }
  }

  last_update = current_time;

  strip.show();
  delay(30); // important to have this!  
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
