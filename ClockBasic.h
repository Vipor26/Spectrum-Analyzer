#include "ClockBase.h"

class ClockBasic : public ClockBase
{
 public:
  ClockBasic();
  void display(SmartMatrix *matrix);
  
  void setUseAmPm(bool flag) { useAmPm = flag; }
 private:
};
