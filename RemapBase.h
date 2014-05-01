
// This class will define the interface for how data is rescaled
//     and reshaped from a 128 double channel FFT to a 16x32 or 32x32
//     display.

#include <arduino.h>

class DataBlock
{
 public:
  //DataBlock(uint16_t numberPoints);
  //DataBlock(const DataBlock &rhs);
  //~DataBlock();
  
  
 private:
  
  uint16_t *XData;
  uint16_t *YData;
};

class FFT_RemapperBase
{
 public:
  FFT_RemapperBase();
  virtual ~FFT_RemapperBase();
  
  //virtual 
  
 private:
  uint16_t *XData;
  uint16_t *YData;
};
