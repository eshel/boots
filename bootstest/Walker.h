#ifndef _WALKER_H_
#define _WALKER_H_

#include "Animation.h"
#include "ColorUtils.h"

class Walker : public Animation {
public:
	Walker(MultiNeoPixel& strip, bool active) : Animation(strip, active) {
		mWrap = true;
		mPosX = 0;
		mPosY = 0;
		mIsActive = false;
		mSizeX = (int8_t)mStrip.getSizeX();
		mSizeY = (int8_t)mStrip.getSizeY();
		mPrevDX = mPrevDY = 0;
		mMaxJumpX = 2;
		mCycleTrailColor = true;

		mColorHead = MultiNeoPixel::Color(255, 255, 255);
		mColorTrailHue = 0;
	}

	void setColorTrailRandom() {
		mColorTrailHue = random(0, 678);
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
	}

	void setColorHead(uint8_t r, uint8_t g, uint8_t b) {
		mColorHead = MultiNeoPixel::Color(r, g, b);
	}

	void setColorTrailHue(uint16_t hue) {
		mColorTrailHue = hue;
	}

protected:
	virtual void performDraw() {
		int8_t px = mPosX;
		int8_t py = mPosY;

		if (mCycleTrailColor) {
			//if (getFrameCount() % 2 == 0) {
				mColorTrailHue++;
			//}
		}

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
			while (mPosX < 0) {
				mPosX += mSizeX;
			}
			while (mPosX >= mSizeX) {
				mPosX -= mSizeX;
			}
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
			while (mPosY < 0) {
				mPosY += mSizeY;
			}
			while (mPosY >= mSizeY) {
				mPosY -= mSizeY;
			}
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
		if (chance2 > 960) {
			do {
				dx = random(-mMaxJumpX, mMaxJumpX);
			} while (dx == 0);
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
			mStrip.setPixelColor(px, py, Wheel(mColorTrailHue));
		}
		mStrip.setPixelColor(mPosX, mPosY, mColorHead);

		mPrevDX = dx;
		mPrevDY = dy;
	}

	virtual void begin() {
		setColorTrailRandom();
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

	void setMaxJumpX(int8_t maxJumpX) {
		mMaxJumpX = maxJumpX;
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
	bool mCycleTrailColor;

	uint32_t mColorHead;
	uint16_t mColorTrailHue;

	int8_t mMaxJumpX;
};


#endif // #ifndef _WALKER_H_
