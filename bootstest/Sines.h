#ifndef __SINES_H__
#define __SINES_H__

#include "Animation.h"
#include "ColorUtils.h"

class Sines : public Animation {
public:
	Sines(MultiNeoPixel& strip, bool active) : Animation(strip, active) {
		mPosX = 0;
		mPosY = 0;
		mIsActive = false;
		mSizeX = mStrip.getSizeX();
		mSizeY = mStrip.getSizeY();
		mPrevDirection = -1;
	}

protected:
	virtual void performDraw() {
		//if (getFrameCount() % 32 == 0) {
		//	clear();
		//	spawn();
		//}
		//step();
		sinRainbow();
	}

public:
	void sinRainbow() {
		for(uint16_t x=0; x < mStrip.getSizeX(); x++) {
			for(uint16_t y=0; y < mStrip.getSizeY(); y++) {
				int val = 64*(sin((x*11 + getFrameCount())/15.f) + sin((y*7 + getFrameCount())/13.f));
				if (val < 0) val = 0;
				int val2 = 384 + 192*(sin((x*7 - getFrameCount())/13.f) + sin((y*11 + getFrameCount())/15.f));
				if (val2 < 0) val2 = 0;
				int val3 = 64*(sin((x*11 + getFrameCount())/27.f) + sin((y*11 - getFrameCount())/17.f));
				if (val3 < 0) val3 = 0;
				mStrip.setPixelColor(x,y, Wheel(val2, max(val-15, 0)));
			}
		}
	}
/*
void sin2Rainbow() {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels()/2; i++) {
      int val = 256*sin((i+j)*PI/100);
      if (val < 0) val = 0;
      uint32_t color = WheelInt((i+j) & 255, val);
      strip.setPixelColor(i, color);
      strip.setPixelColor(NUM_PIXELS-1-i, color);
    }
    strip.show();
    delay(20);
  }  
}*/


	virtual void begin() {
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
	int mPrevDirection;
	uint32_t mFrameNo;
};


#endif // #ifndef __SINES_H__