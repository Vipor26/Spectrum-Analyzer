// This class will define the interface for how data is rescaled
//     and reshaped from a 128 double channel FFT to a 16x32 or 32x32
//     display.
#ifndef REMAP_BASE
#define REMAP_BASE

#include "BasicStructures.h"
#include "SmartMatrix.h"

class RemapBase
{
 public:
  virtual ~RemapBase() {}
  
  virtual void remap(DataBuffer &data) = 0;
  
 private:
};

#endif

