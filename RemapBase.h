// This class will define the interface for how data is rescaled
//     and reshaped from a 128 double channel FFT to a 16x32 or 32x32
//     display.
#ifndef REMAP_BASE
#define REMAP_BASE

#include "arduino.h"
#include "SmartMatrix.h"

class Data
{
  uint16_t X;
  uint16_t Y;
  rgb24 C;
};

class RemapBase
{
 public:
  RemapBase();
  virtual ~RemapBase() {}
  
  virtual void remap(Data data[], uint8_t *dataSize) = 0;
  
 private:
};

#endif

