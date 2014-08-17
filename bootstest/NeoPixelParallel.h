/*--------------------------------------------------------------------
  This file is part of the Adafruit NeoPixel library.

  NeoPixel is free software: you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as
  published by the Free Software Foundation, either version 3 of
  the License, or (at your option) any later version.

  NeoPixel is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with NeoPixel.  If not, see
  <http://www.gnu.org/licenses/>.
  --------------------------------------------------------------------*/

#ifndef ADAFRUIT_NEOPIXEL_H
#define ADAFRUIT_NEOPIXEL_H

#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#include <pins_arduino.h>
#endif

// 'type' flags for LED pixels (third parameter to constructor):
#define NEO_GRB     0x01 // Wired for GRB data order
#define NEO_COLMASK 0x01
#define NEO_KHZ800  0x02 // 800 KHz datastream
#define NEO_SPDMASK 0x02
// Trinket flash space is tight, v1 NeoPixels aren't handled by default.
// Remove the ifndef/endif to add support -- but code will be bigger.
// Conversely, can comment out the #defines to save space on other MCUs.
#ifndef __AVR_ATtiny85__
#define NEO_RGB     0x00 // Wired for RGB data order
#define NEO_KHZ400  0x00 // 400 KHz datastream
#endif

typedef enum {
  ADDRESS_ANY = 0,
  ADDRESS_ALL,
  _ADDRESS_NUM
} EAddressMode;

class MultiNeoPixel {

public:

  // Constructor: number of LEDs, LED type
  MultiNeoPixel(uint8_t sizeX, uint16_t sizeY, uint8_t ledType);
  ~MultiNeoPixel();

  void begin(void);
  
  void show(void);
  void showAll(void);
  void showOne(uint8_t stripIndex);
  
  // This does the actual drawing, once everything has been configured
  void performShow(uint8_t* stripBuffer);
  
  inline uint8_t getSizeX() const {
    return mNumStrips;
  }
  
  inline uint8_t getSizeY() const {
    return mPixelsPerStrip;
  }
  
  inline uint16_t getSizePixels() const {
    return mNumPixels;
  }
  
  void setAddessMode(EAddressMode addressMode) {
    switch (addressMode) {
    case ADDRESS_ANY:
      setModeAny();
      break;
    case ADDRESS_ALL:
    default:
      setModeAll();
    }  
  }
  
  EAddressMode getAddressMode() {
    return mAddressMode;
  }
  
  void clearAll();  
  void divideAll(uint8_t d);
  void addAll(int8_t d);
  
  void setModeAll();
  void setModeAny();
    
  void setPinMask(uint16_t mask);
  void setPin(uint8_t p);
    
  void addPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b);
  
  inline uint8_t* getPixels() const {
    return mPixels;
  }
  
  inline uint16_t numPixels(void) const {
    return mPixelsPerStrip;
  }
  
  inline uint8_t getNumStrips(void) const {
    return mNumStrips;
  }
  
  inline uint16_t getNumAddresses(void) const {
    if (mAddressMode == ADDRESS_ANY) {
      return getSizePixels();
    } else {
      return getSizeY();
    }
  }
  
  inline uint16_t index(uint8_t x, uint8_t y) const {
    return ((uint16_t)x * (uint16_t)mNumStrips) + (uint16_t)y;
  }
  
// Convert separate R,G,B into packed 32-bit RGB color.
// Packed format is always RGB, regardless of LED strand color order.  
  static inline uint32_t Color(uint8_t r, uint8_t g, uint8_t b) {
    return ((uint32_t)r << 16) | ((uint32_t)g <<  8) | b;
  }
  
  inline uint32_t getPixelColor(uint16_t n) const {
    uint16_t ofs = n * 3;
    return (uint32_t)(mPixels[ofs + 2]) |
#ifdef NEO_RGB
      (((mType & NEO_COLMASK) == NEO_GRB) ?
#endif
        ((uint32_t)(mPixels[ofs    ]) <<  8) |
        ((uint32_t)(mPixels[ofs + 1]) << 16)
#ifdef NEO_RGB
      :
        ((uint32_t)(mPixels[ofs    ]) << 16) |
        ((uint32_t)(mPixels[ofs + 1]) <<  8) )
#endif
      ;
  }
  
  inline void setPixelColor(uint8_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b) {
    setPixelColor(index(x, y), r, g, b);
  }
  
  inline void setPixelColor(uint8_t x, uint8_t y, uint32_t c) {
    setPixelColor(index(x, y), c);
  }
  
  inline void setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b) {
    uint8_t *p = &mPixels[n * 3];
#ifdef NEO_RGB
    if((mType & NEO_COLMASK) == NEO_GRB) {
#endif
      *p++ = g;
      *p++ = r;
#ifdef NEO_RGB
    } else {
      *p++ = r;
      *p++ = g;
    }
#endif
    *p = b;
  }

// Set pixel color from 'packed' 32-bit RGB color:
  inline void setPixelColor(uint16_t n, uint32_t c) {
    uint8_t
      r = (uint8_t)(c >> 16),
      g = (uint8_t)(c >>  8),
      b = (uint8_t)c;
    uint8_t *p = &mPixels[n * 3];
#ifdef NEO_RGB
    if((mType & NEO_COLMASK) == NEO_GRB) {
#endif
      *p++ = g;
      *p++ = r;
#ifdef NEO_RGB
    } else {
      *p++ = r;
      *p++ = g;
    }
#endif
    *p = b;
  }  
  
private:
  EAddressMode mAddressMode;
  const uint16_t mPixelsPerStrip;       // Number of RGB LEDs in strip
  const uint16_t mBytesPerStrip;      // Size of 'pixels' buffer below
  const uint16_t mNumPixels;
  const uint16_t mNumBytes;
#if defined(NEO_RGB) || defined(NEO_KHZ400)
  const uint8_t mType;          // Pixel flags (400 vs 800 KHz, RGB vs GRB color)
#endif
  uint8_t* mPixels;        // Holds LED color values (3 bytes each)
  uint8_t mNumStrips;      // Number of parallel LED strips we have connected
  uint32_t mEndTime;       // Latch timing reference
#ifdef __AVR__
  const volatile uint8_t* mPort;         // Output PORT register
  uint8_t mPinMask;       // Output PORT bitmask
#endif



public:

  inline const uint8_t* getCPtrR(uint16_t n) const {
    return    
#ifdef NEO_RGB
        (((mType & NEO_COLMASK) == NEO_GRB) ?
#endif
          &mPixels[n*3 + 1]
#ifdef NEO_RGB
          : &mPixels[n*3] )
#endif
        ;
  }
  
  inline uint8_t* getPtrR(uint16_t n) {
    return (uint8_t*)getCPtrR(n);
  }
  

  inline const uint8_t getPixelR(uint16_t n) const {
    return *getCPtrR(n);
  }
  
  inline void setPixelR(uint16_t n, uint8_t r) {
    *getPtrR(n) = r;
  }
    
  
  inline const uint8_t* getCPtrG(uint16_t n) const {
    return    
#ifdef NEO_RGB
        (((mType & NEO_COLMASK) == NEO_GRB) ?
#endif
          &mPixels[n*3]
#ifdef NEO_RGB
          : &mPixels[n*3 + 1] )
#endif
        ;
  }
  
  inline uint8_t* getPtrG(uint16_t n) {
    return (uint8_t*)getCPtrG(n);
  }
  
  inline const uint8_t getPixelG(uint16_t n) const {
    return *getCPtrG(n);
  }
  
  inline void setPixelG(uint16_t n, uint8_t g) {
    *getPtrG(n) = g;
  }
  
  inline const uint8_t* const getCPtrB(uint16_t n) const {
    return &mPixels[n*3 + 2]; 
  }
  
  inline uint8_t* getPtrB(uint16_t n) {
    return (uint8_t*)getCPtrB(n);
  }
  
  inline const uint8_t getPixelB(uint16_t n) const {
    return *getCPtrB(n);
  }
  
  inline void setPixelB(uint16_t n, uint8_t b) {
    *getPtrB(n) = b;
    
  }
  
};


#endif // ADAFRUIT_NEOPIXEL_H
