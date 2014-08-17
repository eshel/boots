#include "NeoPixelParallel.h"
#include "Particle.h"
#include "ColorUtils.h"

#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050.h"
MPU6050 accelgyro(0x68);

int16_t ax, ay, az;
int16_t gx, gy, gz;


//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
MultiNeoPixel strip = MultiNeoPixel(7, 16, NEO_GRB + NEO_KHZ800);

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

  Serial.begin(38400);
  setupIMU();

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  
  last_update = millis();
}


void setupIMU() {
  // join I2C bus (I2Cdev library doesn't do this automatically)
  Wire.begin();

  // initialize device
  Serial.println("Initializing I2C devices...");
  accelgyro.initialize();

  delay(3000);

  // verify connection
  Serial.println("Testing device connections...");
  Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
}  

static uint32_t sFrameNo = 0;

void loop() {
  strip.setModeAny();
  current_time = millis();
  //do_particles();
  test_pattern();
  //random_blips(1);
  last_update = current_time;
  
  sFrameNo++;
  delay(30); // important to have this!
  
  // read raw accel/gyro measurements from devic
  accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  // these methods (and a few others) are also available
  //accelgyro.getAcceleration(&ax, &ay, &az);
  //accelgyro.getRotation(&gx, &gy, &gz);

  // display tab-separated accel/gyro x/y/z values
  Serial.print("a/g:\t");
  Serial.print(ax); Serial.print("\t");
  Serial.print(ay); Serial.print("\t");
  Serial.print(az); Serial.print("\t");
  Serial.print(gx); Serial.print("\t");
  Serial.print(gy); Serial.print("\t");
  Serial.println(gz);  
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
    strip.addAll(-15);
  }
  
  //delay(random(10, 50));
  delay(23);
  uint8_t pixelCount = 1;//random(0, strip.getNumAddresses());
  for (uint8_t i=0; i<pixelCount; i++) {
    add_random_blip();
  }
  
  strip.show();
}

void test_pattern() {
  draw_test_pattern(1);
}


void draw_test_pattern(uint8_t cleanInBetween) {
  uint16_t maxPixel = strip.getNumAddresses();
  
  if (cleanInBetween) {
    strip.clearAll();
  }
    
  uint32_t c = Wheel(sFrameNo % 768);
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
