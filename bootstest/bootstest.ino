#include "NeoPixelParallel.h"
#include "Particle.h"
#include "ColorUtils.h"
#include "Motion.h"
#include "Walker.h"

#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050.h"


#define LED_PIN  17
#define LED_ON   false
#define LED_OFF  true

//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
MultiNeoPixel strip = MultiNeoPixel(7, 16, NEO_GRB + NEO_KHZ800);

ParticleSystem particles(strip);

static uint32_t sFrameNo = 0;

Motion motionSensor;
Walker walker(strip);

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

  motionSensor.begin();
  bool motionOK = motionSensor.test();
  Serial.println(motionOK ? "Motion init successful" : "Motion init failed");  

  // configure Arduino LED for
  pinMode(LED_PIN, OUTPUT);

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

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
    digitalWrite(LED_PIN, LED_ON);
    onStep();
  } else {
    digitalWrite(LED_PIN, LED_OFF);
  }

}


void loop() {
  motionSensor.sample();
  motionSensor.print();

  doMotion();

  strip.setModeAny();
  current_time = millis();
  //do_particles();

  //strip.addAll(-25);
  strip.multAll(4, 5);

  test_pattern();

  do_walker();
  //random_blips(-20, 3);

  last_update = current_time;
  
  sFrameNo++;

  strip.show();
  delay(30); // important to have this!  
}


#define INC_MOD(x, lim) (x)++; if ((x) >= (lim)) x=0;


void add_random_blip() {
  uint16_t maxPixel = strip.getNumAddresses();
  uint16_t colorOffset = random(0, (256*3));
  uint32_t color = Wheel(colorOffset);
  uint16_t pixelIndex = random(0, maxPixel);
  
  strip.setPixelColor(pixelIndex, color);
}  

void random_blips(int8_t minPixels, int8_t maxPixels) {
  //delay(random(10, 50));
  //delay(23);
  
  int8_t newPixels = random(minPixels, maxPixels);
  if (newPixels < 0) {
    newPixels = 0;
  }
  for (uint8_t i=0; i<(uint8_t)newPixels; i++) {
    add_random_blip();
  }
}

void test_pattern() {
  static uint16_t pixelIndex = 0;
  uint16_t maxPixel = strip.getNumAddresses();
  
  uint32_t c = Wheel(sFrameNo % 768);
  strip.setPixelColor(pixelIndex, c);

  pixelIndex++;
  if (pixelIndex >= maxPixel) {
    pixelIndex = 0;
  }
}


void do_particles() {
  strip.clearAll();
  particles.runFrame(current_time);
  strip.show();
}

void do_walker() {
  if (sFrameNo % 32 == 0) {
    walker.clear();
    walker.spawn();
  }
  walker.step();
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
