
#ifndef __PARTICLE_H__
#define __PARTICLE_H__

#include "NeoPixelParallel.h"
#include "ColorUtils.h"
#include "Animation.h"

#define MAX_PARTICLES 5
#define NUM_COLUMNS 7
#define NUM_ROWS 16
#define SPAWN_TIME 1311

// units are pixels per 1000 milliseconds 
#define PARTICLE_SPEED (930*16)
// units are trailers per 1000 milliseconds (note: correct model is poisson distribution, you won't find it here)
#define TRAILER_RATE 4

class ParticleSystem : public Animation {
public:
  ParticleSystem(MultiNeoPixel& ledStrip, bool active) : Animation(ledStrip, active),
    next_spawn_index(0)
  {
  }
  
  void runFrame() {
    unsigned long current_time = getTime();
    if (current_time - last_spawn_time > SPAWN_TIME) {
      spawnParticle(current_time, next_spawn_index);
      next_spawn_index = (next_spawn_index + 1) % NUM_COLUMNS;
    }
    int time_delta = current_time - last_update_time;
    for(uint8_t col = 0; col < NUM_COLUMNS; col++) {
      runColumn(col, time_delta);
    }
    last_update_time = current_time;
  }

  
protected:  
    virtual void performDraw() {
      runFrame();
    }

    virtual void begin() {
      last_update_time = last_spawn_time = getTime();
      for(uint8_t col = 0; col < NUM_COLUMNS; col++) {
        columns[col].begin();
      }
    }

    virtual void clear() {
      if (mIsActive) {
        begin();
      }
      mIsActive = 0;
    }

    void runColumn(uint8_t col, int time_delta) {
      // TODO: handle overlapping particles
      // delta xfor pulse - speed at 16 pixels per second
      int pos_delta = ((uint32_t)time_delta * PARTICLE_SPEED) / 1000;
    
      for(int i = 0; i < MAX_PARTICLES; i++) {
        Column::Particle &particle = columns[col].particles[i];
        if (!particle.alive) continue;
        
        //Serial.print(i); Serial.print(") pos = "); Serial.print(particles[i].pos); Serial.print(" r = "); Serial.print(particles[i].r);
        // draw pulse
        drawParticle(particle, i);
        
        // see if we want to spawn a trailer
        int minX = ((particle.pos - particle.radius) >> 8) - 1;
        if (minX >= 0 && minX <= (NUM_ROWS - 1)) {
          if (random(0, 1000) < time_delta * TRAILER_RATE) {
            mStrip.setPixelColor(col, (NUM_ROWS-1) - minX, particle.r, particle.g, particle.b);
          }
        }
        //Serial.print(" pix=");
        //Serial.println();
        // move pulse - speed at 16 pixels per 257/1000th of a second
        particle.pos += pos_delta;
        if (particle.pos  >= (256*16 + 2*particle.radius)) {
          Serial.print("killin' particle: "); Serial.println(particle.pos);
          particle.alive = false;
        }
      }

    }

  
protected:
    // particles array
  class Column {
  public:
    struct Particle {
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
    //void spawnBigParticle();
    //void spawnSmallParticle();
    inline void begin() {
       for(int i = 0; i < MAX_PARTICLES; i++) {
        particles[i].alive = false;
      }     
    }

    inline void clear() {
      begin();
    }

    //Particle &spawnParticle();


  } columns[NUM_COLUMNS];

  inline void drawParticle(const Column::Particle &particle, int col) {
    for(int x = (max(0,particle.pos - particle.radius)& (~0xff)); x < (min(256*16, particle.pos + particle.radius + 255) & (~0xff)); x += 256) {
      
      // shift-left by 4 is to scale the distance from fixed-point pixels ( [0..16) with 256 steps per whole number)
      // to fixed point ([0..1) with 256 levels. This is important because otherwise we overflow 16-bit integers in the next
      // multiplication
      int closeness = max(particle.radius - abs(particle.pos - x), 0) >> 4;
      //Serial.print(x); Serial.print(", ");
      mStrip.addPixelColor(col, (NUM_ROWS - 1) - (x >> 8), (particle.r * closeness) / (particle.radius >> 4), (particle.g * closeness)  / (particle.radius >> 4), (particle.b * closeness) / (particle.radius >> 4));
      //strip.setPixelColor(x >> 8, 124,221,0);
    }
  }

  void spawnParticle(const unsigned long current_time, unsigned int col) {
    Column::Particle &particle = columns[col].particles[next_spawn_index];
    last_spawn_time = current_time;
    particle.alive=true;
    particle.radius = 31*16;
    particle.pos = -particle.radius;

    particle.color = Wheel((current_time >> 5) % 768);
    next_spawn_index = (next_spawn_index + 1) % MAX_PARTICLES;
  }


  // particle spawning state
  unsigned long last_spawn_time;
  unsigned long last_update_time;
  uint8_t next_spawn_index;


};

#endif

