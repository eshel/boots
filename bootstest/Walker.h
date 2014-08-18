#ifndef _WALKER_H_
#define _WALKER_H_

#include "Animation.h"

class Walker : public Animation {
public:
	Walker(MultiNeoPixel& strip, bool active) : Animation(strip, active) {
		mWrap = false;
		mPosX = 0;
		mPosY = 0;
		mIsActive = false;
		mSizeX = (int8_t)mStrip.getSizeX();
		mSizeY = (int8_t)mStrip.getSizeY();
		mPrevDX = mPrevDY = 0;

		mColorHead = MultiNeoPixel::Color(255, 255, 255);
		mColorTrail = MultiNeoPixel::Color(0, 255, 0);
	}

	void setIsWrapping(bool isWrapping) {
		mWrap = isWrapping;
	}

	void spawn() {
		spawn(random(0, mSizeX), random(0, mSizeY));
	}

	void spawn(uint8_t x, uint8_t y) {
		clear();
		mPosX = x;
		mPosY = y;
		mIsActive = true;
		mStrip.setPixelColor(x, y, mColorHead);
		Serial.print("Spawning walker: [");
		Serial.print(mPosX);
		Serial.print(",");
		Serial.print(mPosY);
		Serial.print("] of [");
		Serial.print(mSizeX);
		Serial.print(",");
		Serial.print(mSizeY);
		Serial.println("]");
	}

	void setColorHead(uint8_t r, uint8_t g, uint8_t b) {
		mColorHead = MultiNeoPixel::Color(r, g, b);
	}


	void setColorTrail(uint8_t r, uint8_t g, uint8_t b) {
		mColorTrail = MultiNeoPixel::Color(r, g, b);
	}

protected:
	virtual void performDraw() {
		uint8_t px = mPosX;
		uint8_t py = mPosY;

		step();

		Serial.print('[');
		Serial.print(px);
		Serial.print(',');
		Serial.print(py);
		Serial.print("] --> [");
		Serial.print(mPosX);
		Serial.print(',');
		Serial.print(mPosY);
		Serial.println(']');
	}

public:
	inline bool insideBounds() {
		return ((mPosX >= 0) && (mPosY >= 0) && (mPosX < mSizeX) && (mPosY < mSizeY));
	}

	inline void fixX() {
		if (mWrap) {
			mPosX = mPosX % mSizeX;
		} else {
			if (mPosX < 0) {
				mPosX = 0;
			} else if (mPosX >= mSizeX) {
				mPosX = mSizeX - 1;
			}
		}
	}

	inline void fixY() {
		if (mWrap) {
			mPosY = mPosY % mSizeY;
		} else {
			if (mPosY < 0) {
				mPosY = 0;
			} else if (mPosY >= mSizeY) {
				mPosY = mSizeY - 1;
			}
		}
	}

	inline void fix() {
		fixX();
		fixY();
	}

	static int8_t newDirection() {
		int8_t d = random(0, 2);
		if (d == 0) {
			return -1;
		} else {
			return 1;
		}
	}	

	void step() {
		int8_t dx, dy;
		int chance = random(0, 100);
		if (chance < 95) {
			dy = mPrevDY;
		} else {
			dy = newDirection();
		}

		int chance2 = random(0, 1000);
		if (chance2 < 980) {
			dx = newDirection();
		} else {
			dx = 0;
		}

		step(dx, dy);
	}

	void step(int8_t dx, int8_t dy) {
		if (!mIsActive) {
			return;
		}
		int8_t px = mPosX;
		int8_t py = mPosY;

		mPosX += dx;
		mPosY += dy;
		fix();

		if ((px != mPosX) || (py != mPosY)) {
			mStrip.setPixelColor(px, py, mColorTrail);
		}
		mStrip.setPixelColor(mPosX, mPosY, mColorHead);

		mPrevDX = dx;
		mPrevDY = dy;
	}

	virtual void begin() {
		spawn();
	}

	virtual void clear() {
		if (mIsActive) {
			mStrip.setPixelColor(mPosX, mPosY, 0);
		}
		mPosX = 0;
		mPosY = 0;
		mIsActive = 0;
	}

private:
	int8_t mPosX;
	int8_t mPosY;
	int8_t mSizeX;
	int8_t mSizeY;
	bool mIsActive;
	int8_t mPrevDX, mPrevDY;
	uint32_t mFrameNo;
	bool mWrap;

	uint32_t mColorHead;
	uint32_t mColorTrail;
};


#endif // #ifndef _WALKER_H_
