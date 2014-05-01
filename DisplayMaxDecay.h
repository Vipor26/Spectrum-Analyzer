#include "SpectrumDisplayBase.h"

class MaxDecayDisplay : public SpectrumDisplayBase
{
 public:
  MaxDecayDisplay();
  
  void setDecayFunction();
  void setPainter();
  
  void display(uint16_t &output, SmartMatrix *matrix);

 private:
  uint16_t m_max[128] __attribute__ ((aligned (4)));
};
