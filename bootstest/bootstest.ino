#include "NeoPixelParallel.h"
#include "Particle.h"
#include "ColorUtils.h"

#define STRIPS_NUM  7
#define STRIP_LENGTH 16

//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
MultiNeoPixel strip = MultiNeoPixel(7, STRIP_LENGTH, NEO_GRB + NEO_KHZ800);

ParticleSystem particles(strip);

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

  strip.begin();
  //strip.setModeAll();
  strip.show(); // Initialize all pixels to 'off'
  
  last_update = millis();
}

static uint32_t sFrameNo = 0;

void loop() {
  current_time = millis();
  //do_particles();
  //test_pattern();
  random_blips(1);
  last_update = current_time;
  
  sFrameNo++;
  delay(10); // important to have this!
}

static uint8_t x = 0;
static uint8_t y = 0;
static int wheelPos = 0;

static uint16_t pixelIndex = 0;
static uint8_t fullIterNum = 0;

#define INC_MOD(x, lim) (x)++; if ((x) >= (lim)) x=0;


void add_random_blip() {
  uint16_t maxPixel = strip.getNumAddresses();
  uint16_t colorOffset = random(0, (256*3));
  uint32_t color = Wheel(colorOffset);
  uint16_t pixelIndex = random(0, maxPixel);
  
  strip.setPixelColor(pixelIndex, color);
}  

void random_blips(uint8_t cleanInBetween) {
  if (cleanInBetween) {
    strip.clearAll();
  }
  
  delay(random(100, 500));
  uint8_t pixelCount = random(0, 28);
  for (uint8_t i=0; i<pixelCount; i++) {
    add_random_blip();
  }
  
  strip.show();
}

void test_pattern() {
  strip.setModeAny();
  draw_test_pattern(1);
}


void draw_test_pattern(uint8_t cleanInBetween) {
  uint16_t maxPixel = strip.getNumAddresses();
  
  if (cleanInBetween) {
    strip.clearAll();
  }
    
  uint32_t c = Wheel(fullIterNum * 30);
  strip.setPixelColor(pixelIndex, c);
  strip.show();

  pixelIndex++;
  if (pixelIndex >= maxPixel) {
    pixelIndex = 0;
    fullIterNum++;
  }
}


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
