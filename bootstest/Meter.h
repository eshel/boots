#ifndef _METER_H_
#define _METER_H_

#include "Animation.h"
#include "ColorUtils.h"
#include "Motion.h"

class Meter : public Animation {
public:
	Meter(MultiNeoPixel& strip, Motion& motion, bool active) : Animation(strip, active), mMotion(motion) {
		mCurrentPower = 0;
		setIsLog(true);
		setPowerRange(1200, 5000);
	}

	void setIsLog(bool isLog) {
		mIsLog = isLog;
	}

	void setPowerRange(uint16_t minPower, uint16_t maxPower) {
		mMinPower = minPower;
		mMaxPower = maxPower;
	}

	virtual void begin() {

	}

	virtual void clear() {

	}

	int16_t filter(int16_t value) {
		if (value < 0) {
			value = -value;
		}
		return value;
	}

	virtual void update() {
		int16_t value = mMotion.getSample().apower;
		value = filter(value);
		mCurrentPower = value;
	}

	uint32_t getColor(uint8_t yVal, uint8_t stripsNum) {
		uint16_t part = (yVal * 768) / mStrip.getSizeY();
		return Wheel(part);
	}

	uint16_t calcLevel(uint16_t currentPower) {
		int16_t lvl;
		if (mIsLog) {
			float minLog = logf((float)mMinPower);
			float maxLog = logf((float)mMaxPower);
			float powerStep = ((maxLog - minLog) / (float)mStrip.getSizeY());
			float currentLog = logf((float)currentPower); 
			float relativeLog = currentLog - minLog;
			lvl = (int16_t)(relativeLog / powerStep);
		} else {
			int16_t relativePower = mCurrentPower - mMinPower;			
			int16_t powerStep = (mMaxPower - mMinPower) / mStrip.getSizeY();
			lvl = relativePower / powerStep;
		}
		if (lvl > mStrip.getSizeY() - 1) {
			lvl = mStrip.getSizeY() - 1;
		} else if (lvl < 0) {
			lvl = 0;
		}

		return (uint16_t)lvl;
	}

protected:
	virtual void performDraw() {
		update();

		uint16_t xsize = mStrip.getSizeX();
		uint16_t powerStep = ((mMaxPower - mMinPower) / mStrip.getSizeY());

		int8_t ysize = calcLevel(mCurrentPower);

		for (uint8_t x = 0; x < xsize; x++) {
			if ((x == 0) || (x == 4)) {
				for (uint8_t y = 0; y < ysize; y++) {
					mStrip.setPixelColor(x, y, getColor(y, ysize));			
				}
			}
		}
	}	

private:
	Motion& mMotion;
	int16_t mCurrentPower;
	uint16_t mMinPower;
	uint16_t mMaxPower;
	float mLogMinPower;
	float mLogMaxPower;
	bool mIsLog;
};

#endif // #ifndef _METER_H_