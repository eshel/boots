#ifndef _METER_H_
#define _METER_H_

#include "Animation.h"
#include "ColorUtils.h"

class Meter : public Animation {
public:
	Meter(MultiNeoPixel& strip, bool active) : Animation(strip, active) {
		mLastPower = mCurrentPower = 0;
		setIsLog(false);
		setPowerRange(800, 1600);
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

	virtual void update(int16_t value) {
		filter(value);
		mLastPower = mCurrentPower;
		mCurrentPower = value;
	}

	uint32_t getColor(uint8_t yVal, uint8_t stripsNum) {
		uint16_t part = (yVal * 768) / mStrip.getSizeY();
		return Wheel(part);
	}

	uint16_t calcLevel(uint16_t currentPower) {
		int16_t lvl;
		int16_t powerStep = (mMaxPower - mMinPower) / mStrip.getSizeY();

		if (mIsLog) {
			lvl = 0;
		} else {
			lvl = (mCurrentPower - mMinPower) / powerStep;
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
	int16_t mLastPower;
	int16_t mCurrentPower;
	uint16_t mMinPower;
	uint16_t mMaxPower;
	float mLogMinPower;
	float mLogMaxPower;
	bool mIsLog;
};

#endif // #ifndef _METER_H_