#ifndef _MOTION_H_
#define _MOTION_H_

#include "MPU6050.h"

class Motion {
public:
	Motion() : mSensor(0x68) {
		ax = ay = az = 0;
		gx = gy = gz = 0;
		apower = 0;
	}

	~Motion() {

	}

	void begin() {
		ax = ay = az = 0;
		gx = gy = gz = 0;
		apower = 0;
  		mSensor.initialize();
  		mSensor.setFullScaleAccelRange(MPU6050_ACCEL_FS_8);
	}

	void sample() {
		// read raw accel/gyro measurements from device
		mSensor.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

		// 1G = 1024 (when in 8G mode)
		ax >>= 2;
		ay >>= 2;
		az >>= 2;

		float fx = (float)ax;
		float fy = (float)ay;
		float fz = (float)az;

		apower = (int16_t)sqrt(fx*fx + fy*fy + fz*fz);
	}

	void print() const {
		// display tab-separated accel/gyro x/y/z values
		Serial.print("a/g:\t");
		Serial.print(ax); Serial.print('\t');
		Serial.print(ay); Serial.print('\t');
		Serial.print(az); Serial.print('\t');
		Serial.print(apower); Serial.print('\t');
		Serial.print(gx); Serial.print('\t');
		Serial.print(gy); Serial.print('\t');
		Serial.print(gz); Serial.print('\t');
		Serial.println();
	}

	bool test() {
		return mSensor.testConnection() ? true : false;
	}

	inline int16_t getAPower() const {
		return apower;
	}

	inline int16_t getAX() const {
		return ax;
	}

	inline int16_t getAY() const {
		return ay;
	}

	inline int16_t getAZ() const {
		return az;
	}

	inline int16_t getGX() const {
		return gx;
	}

	inline int16_t getGY() const {
		return gy;
	}

	inline int16_t getGZ() const {
		return gz;
	}

private:
	MPU6050 mSensor;

	int16_t ax, ay, az;
	int16_t apower;
	int16_t gx, gy, gz;
};

#endif // #ifndef _MOTION_H_

