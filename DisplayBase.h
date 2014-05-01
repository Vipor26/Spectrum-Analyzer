#include <SmartMatrix.h>
#include "Arduino.h"

class FFT_DisplayBase
{
 public:
  FFT_DisplayBase();
  virtual ~FFT_DisplayBase();
  
  virtual void display(uint16_t *FFT_Data, SmartMatrix *matrix) = 0;
  
 private:
};
