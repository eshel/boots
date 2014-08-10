#include "NeoPixelParallel.h"
#include "Particle.h"
#include "ColorUtils.h"
#define FULL_MASK  0x07

uint8_t mask = 0x07;

#define STRIP_LENGTH 16
// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(STRIP_LENGTH, NEO_GRB + NEO_KHZ800);

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
  strip.begin();
  strip.show(mask); // Initialize all pixels to 'off'
  last_update = millis();
}


void loop() {
  /*
  // Some example procedures showing how to display to the pixels:
  colorWipe(strip.Color(255, 0, 0), 50); // Red
  colorWipe(strip.Color(0, 255, 0), 50); // Green
  colorWipe(strip.Color(0, 0, 255), 50); // Blue
  // Send a theater pixel chase in...
  theaterChase(strip.Color(127, 127, 127), 50); // White
  theaterChase(strip.Color(127,   0,   0), 50); // Red
  theaterChase(strip.Color(  0,   0, 127), 50); // Blue
*/
  //rainbow(10);
  //rainbowCycle(10);
  
  //theaterChaseRainbow(50);
  current_time = millis();
  do_particles();
  last_update = current_time;
  delay(10); // important to have this!
  
}


  
void do_particles() {

  // TODO: move to function
  // clear all pixels
  for(int i = 0; i < STRIP_LENGTH; i++) {
    strip.setPixelColor(i, 0);
  }
  
  particles.runFrame(current_time);

  strip.show(0x7f);
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<768; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) % 768));
    }
    strip.show(mask);
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
    strip.show(mask);
    delay(wait);
  }
}



