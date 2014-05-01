#include <SmartMatrix.h>
#include "Arduino.h"

class DisplayBase
{
 public:
  DisplayBase();
  virtual ~DisplayBase();
  
  virtual void display(uint16_t *FFT_Data, SmartMatrix *matrix) = 0;
  
 private:
};
