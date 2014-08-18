

#ifndef __IMU_H__
#define __IMU_H__

#include "FastRunningMedian.h"

class MotionDetector {
public:
	// initialize MPU
	void setup();
	// read state from device, 
	void update();

	static MotionDetector& getInstance();

                

private:


  FastRunningMedian<float, 5> acceleration;  

  MotionDetector() {
    q[0] = 1.0f;
    q[1] = q[2] = q[3] = 0.0f;
    deltat = 0.0f;
    delt_t = 0;
    count = 0;
    mcount = 0;
    lastUpdate = 0;
    now = 0;
  }

    int16_t a1, a2, a3, g1, g2, g3, m1, m2, m3;     // raw data arrays reading
    uint16_t count;  // used to control display output rate
    uint16_t delt_t; // used to control display output rate
    uint16_t mcount; // used to control display output rate
    uint8_t MagRate;     // read rate for magnetometer data
    
    float pitch, yaw, roll;
    float deltat;        // integration interval for both filter schemes
    uint32_t lastUpdate; // used to calculate integration interval
    uint32_t now;        // used to calculate integration interval
    
    float ax, ay, az, gx, gy, gz, mx, my, mz; // variables to hold latest sensor data values 
    float q[4];    // vector to hold quaternion

};

#endif
