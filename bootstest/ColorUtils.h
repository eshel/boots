#ifndef __COLORUTILS_H__
#define __COLORUTILS_H__

#include "NeoPixelParallel.h"

// Input a value 0 to 767 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(int WheelPos) {
  if(WheelPos < 256) {
   return MultiNeoPixel::Color(WheelPos, 255 - WheelPos, 0);
  } else if(WheelPos < 512) {
   WheelPos -= 256;
   return MultiNeoPixel::Color(255 - WheelPos, 0, WheelPos);
  } else {
   WheelPos -= 512;
   return MultiNeoPixel::Color(0, WheelPos, 255 - WheelPos);
  }
}

#endif
