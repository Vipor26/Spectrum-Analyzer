
#include "SpectrumDisplayBase.h"

class MaxDecayDisplay : SpectrumDisplayBase
{
 public:
  void setDecayFunction();
	
  void display(uint16_t &output, SmartMatrix *matrix);

 private:
  uint16_t output[128] __attribute__ ((aligned (4)));
};
