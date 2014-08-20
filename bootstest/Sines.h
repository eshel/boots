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
		sinRainbow();
	}

public:
	void sinRainbow() {
		uint32_t currentTime = millis();
		for(uint16_t x=0; x < mStrip.getSizeX(); x++) {
			for(uint16_t y=0; y < mStrip.getSizeY(); y++) {
				int val = 64*(sin((x*131 + currentTime)/151.f) + sin((y*71 + currentTime)/131.f));
				int val2 = 384 + 192*(sin((x*113 - currentTime)/131.f) + sin((y*31 + currentTime)/151.f));
				//if (val2 < 0) { val2 = 0; Serial.println("fuck small"); }
				//if (val2 >= 768) Serial.println("fuck big");
				//int val3 = 64*(sin((x*11 + getFrameCount())/27.f) + sin((y*11 - getFrameCount())/17.f));
				//if (val3 < 0) val3 = 0;
				mStrip.setPixelColor(x,y, Wheel(val2, max(val-15, 0)));
			}
		}
	}
	virtual void begin() {
	}

	virtual void clear() {
		mIsActive = 0;
	}

	//inline byte sine(byte offset, )

private:
	prog_uchar PROGMEM sinetable[256] = {
	  128,131,134,137,140,143,146,149,152,156,159,162,165,168,171,174,
	  176,179,182,185,188,191,193,196,199,201,204,206,209,211,213,216,
	  218,220,222,224,226,228,230,232,234,236,237,239,240,242,243,245,
	  246,247,248,249,250,251,252,252,253,254,254,255,255,255,255,255,
	  255,255,255,255,255,255,254,254,253,252,252,251,250,249,248,247,
	  246,245,243,242,240,239,237,236,234,232,230,228,226,224,222,220,
	  218,216,213,211,209,206,204,201,199,196,193,191,188,185,182,179,
	  176,174,171,168,165,162,159,156,152,149,146,143,140,137,134,131,
	  128,124,121,118,115,112,109,106,103,99, 96, 93, 90, 87, 84, 81, 
	  79, 76, 73, 70, 67, 64, 62, 59, 56, 54, 51, 49, 46, 44, 42, 39, 
	  37, 35, 33, 31, 29, 27, 25, 23, 21, 19, 18, 16, 15, 13, 12, 10, 
	  9,  8,  7,  6,  5,  4,  3,  3,  2,  1,  1,  0,  0,  0,  0,  0,  
	  0,  0,  0,  0,  0,  0,  1,  1,  2,  3,  3,  4,  5,  6,  7,  8,  
	  9,  10, 12, 13, 15, 16, 18, 19, 21, 23, 25, 27, 29, 31, 33, 35, 
	  37, 39, 42, 44, 46, 49, 51, 54, 56, 59, 62, 64, 67, 70, 73, 76, 
	  79, 81, 84, 87, 90, 93, 96, 99, 103,106,109,112,115,118,121,124
	};
};


#endif // #ifndef __SINES_H__