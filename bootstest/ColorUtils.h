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

uint32_t Wheel(int WheelPos, byte intensity) {
  if(WheelPos < 256) {
   return MultiNeoPixel::Color(WheelPos * intensity / 255, (255 - WheelPos) * intensity / 255, 0);
  } else if(WheelPos < 512) {
   WheelPos -= 256;
   return MultiNeoPixel::Color((255 - WheelPos) * intensity / 255, 0, WheelPos * intensity / 255);
  } else {
   WheelPos -= 512;
   return MultiNeoPixel::Color(0, WheelPos * intensity / 255, (255 - WheelPos) * intensity / 255);
  }
}

// position 0 -> color 1
// position 255 -> color 2
// linearly between them
uint32_t lerp(uint32_t color1, uint32_t color2, byte position) {
	uint8_t r = ((color1 & 0xFF) * position + (color2 & 0xFF) * (255 - position)) >> 8;
	uint8_t g = ((color1 >> 8 & 0xFF) * position + (color2 >> 8 & 0xFF) * (255 - position)) >> 8;
	uint8_t b = ((color1 >> 16 & 0xFF) * position + (color2 >> 16 & 0xFF) * (255 - position)) >> 8;
	return r | (g << 8) | (b << 16);
}


#endif
