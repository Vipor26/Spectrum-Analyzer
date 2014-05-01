
// This class will define the interface for how data is rescaled
//     and reshaped from a 128 double channel FFT to a 16x32 or 32x32
//     display.

#include <arduino.h>

class DataBlock
{
  uint16_t XData;
  uint16_t YData;
};

class RemapBase
{
 public:
  FFT_RemapperBase();
  virtual ~FFT_RemapperBase() {}
  
  virtual void remap(DataBlock *data, uint8_t *dataSize) = 0;
  
 private:
};
