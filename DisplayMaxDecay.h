#include "DisplayBase.h"

class DisplayMaxDecay : public DisplayBase
{
 public:
  DisplayMaxDecay();
  
  void setDecayFunction();
  void setPainter();
  
  void display(uint16_t &output, SmartMatrix *matrix);

 private:
  uint16_t m_max[128] __attribute__ ((aligned (4)));
};
