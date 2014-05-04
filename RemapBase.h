// This class will define the interface for how data is rescaled
//     and reshaped from a 128 double channel FFT to a 16x32 or 32x32
//     display.
#ifndef REMAP_BASE
#define REMAP_BASE

#include "arduino.h"
#include "SmartMatrix.h"

struct Data
{
  uint16_t X;
  uint16_t Y;
  rgb24 C;
};


struct DataBuffer
{
  Data data[128];
  uint8_t size;
};

class RemapBase
{
 public:
  //virtual RemapBase( const RemapBase& ) {} ??
  virtual ~RemapBase() {}
  
  virtual void remap(DataBuffer &data) = 0;
  
 private:
};

#endif

