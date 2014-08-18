#ifndef _WALKER_H_
#define _WALKER_H_

#include "NeoPixelParallel.h"

class Walker {
public:
	Walker(MultiNeoPixel& strip) : mStrip(strip) {
		mPosX = 0;
		mPosY = 0;
		mIsActive = false;
		mSizeX = mStrip.getSizeX();
		mSizeY = mStrip.getSizeY();
		mStepsCount = 0;
	}

	void spawn() {
		spawn(random(0, mSizeX), random(0, mSizeY));
	}

	void spawn(uint8_t x, uint8_t y) {
		mPosX = x;
		mPosY = y;
		mIsActive = true;
		mStrip.setPixelColor(x, y, MultiNeoPixel::WHITE);
	}

	static int randomDirection() {
		int direction = 0;
		int chance = random(0, 100);
		if (chance < 90) {
			direction = random(2, 4);
		} else {
			direction = random(0, 2);
		}
	}

	void step() {
		int direction;
		if (mPrevDirection == -1) {
			direction = randomDirection();
		} else {
			int chance = random(0, 100);
			if (chance < 95) {
				direction = mPrevDirection;
			} else {
				direction = randomDirection();
			}
		}
		step(direction);
	}

	void step(int direction) {
		if (!mIsActive) {
			return;
		}
		int8_t px = mPosX;
		int8_t py = mPosY;

		switch (direction) {
			/*
		case 0:
			Serial.println("X+");
			mPosX++;
			break;
		case 1:
			Serial.println("X-");
			mPosX--;
			break;
			*/
		case 2:
			Serial.println("Y+");
			mPosY++;
			break;
		case 3:
			Serial.println("Y-");
			mPosY--;
			break;
		}

		if (mPosX < 0) {
			mPosX = 0;
		} else if (mPosX >= mSizeX) {
			mPosX = mSizeX - 1;
		}
		if (mPosY < 0) {
			mPosY = 0;
		} else if (mPosY >= mSizeY) {
			mPosY = mSizeY - 1;
		}

		if ((px != mPosX) || (py != mPosY)) {
			mStrip.setPixelColor(mPosX, mPosY, 255, 255, 255);
			mStrip.setPixelColor(px, py, 0, 255, 0);
		}

		mPrevDirection = direction;
		mStepsCount++;
	}

	void clear() {
		if (mIsActive) {
			mStrip.setPixelColor(mPosX, mPosY, 0);
		}
		mPosX = 0;
		mPosY = 0;
		mIsActive = 0;
		mStepsCount = 0;
	}

private:
	MultiNeoPixel& mStrip;
	int8_t mPosX;
	int8_t mPosY;
	int8_t mSizeX;
	int8_t mSizeY;
	uint16_t mStepsCount;
	bool mIsActive;
	int mPrevDirection = -1;
};


#endif // #ifndef _WALKER_H_
