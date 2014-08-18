
#ifndef __PARTICLE_H__
#define __PARTICLE_H__

#include "NeoPixelParallel.h"
#include "ColorUtils.h"

#define MAX_PARTICLES 10
#define SPAWN_TIME 10311

class ParticleSystem {
public:
  ParticleSystem(MultiNeoPixel& ledStrip) :
    strip(ledStrip), last_spawn_time(millis()), next_spawn_index(0)
  {
    last_update_time = last_spawn_time;
    for(int i = 0; i < MAX_PARTICLES; i++) {
      particles[i].alive = false;
    }
  }
  
  void runFrame(unsigned long current_time) {
    if (current_time - last_spawn_time > SPAWN_TIME) {
      //Serial.print("spawning: ");
      //Serial.println(current_time);
      spawnParticle(current_time);
      //spawn_pulse();
    }
    
    // TODO: handle overlapping particles
    int time_delta = current_time - last_update_time;
    // delta xfor pulse - speed at 16 pixels per second
    int pos_delta = 15;//((uint32_t)time_delta * 1130*16) / 1000;
  
    for(int i = 0; i < MAX_PARTICLES; i++) {
      if (!particles[i].alive) continue;
      
      //Serial.print(i); Serial.print(") pos = "); Serial.print(particles[i].pos); Serial.print(" r = "); Serial.print(particles[i].r);
      // draw pulse
      drawParticle(i);
      //Serial.print(" pix=");
      //Serial.println();
      // move pulse - speed at 16 pixels per 257/1000th of a second
      particles[i].pos += pos_delta;
      if (particles[i].pos  >= (256*16 + 2*particles[i].radius)) {
        Serial.print("killin' particle: "); Serial.println(particles[i].pos);
        particles[i].alive = false;
      }
    }
    last_update_time = current_time;
  }

  void spawnParticle(const unsigned long current_time) {
    last_spawn_time = current_time;
    particles[next_spawn_index].alive=true;
    particles[next_spawn_index].radius = 51*16;
    particles[next_spawn_index].pos = -particles[next_spawn_index].radius;

    particles[next_spawn_index].color = Wheel((current_time >> 5) % 768);
    next_spawn_index = (next_spawn_index + 1) % MAX_PARTICLES;
  }


protected:  
  
  void drawParticle(int i) {
    for(int x = (max(0,particles[i].pos - particles[i].radius)& (~0xff)); x < (min(256*16, particles[i].pos + particles[i].radius + 255) & (~0xff)); x += 256) {
      
      // shift-left by 4 is to scale the distance from fixed-point pixels ( [0..16) with 256 steps per whole number)
      // to fixed point ([0..1) with 256 levels. This is important because otherwise we overflow 16-bit integers in the next
      // multiplication
      int closeness = max(particles[i].radius - abs(particles[i].pos - x), 0) >> 4;
      //Serial.print(x); Serial.print(", ");
      strip.setPixelColor(x >> 8, (particles[i].r * closeness) / (particles[i].radius >> 4), (particles[i].g * closeness)  / (particles[i].radius >> 4), (particles[i].b * closeness) / (particles[i].radius >> 4));
      //strip.setPixelColor(x >> 8, 124,221,0);
    }
  }

  
protected:

  // reference to pixel strip for drawing
  MultiNeoPixel& strip;
  
  // particle spawning state
  unsigned long last_spawn_time;
  unsigned long last_update_time;
  unsigned int next_spawn_index;

  // particles array
  struct {
    boolean alive;
    int16_t pos; // fixed-point, 8.8
    int16_t radius; // fixed-point, 8.8
    union {
      uint32_t color;
      struct {
        byte r;
        byte g;
        byte b;
      };
    };
  } particles[MAX_PARTICLES];
};

#endif

