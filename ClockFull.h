#include "ClockBase.h"

class ClockFull : public ClockBase
{
 public:
  ClockFull();
  void display(SmartMatrix *matrix);

  void setUseAmPm(bool flag) { useAmPm = flag; }
 private:
};
