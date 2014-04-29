#include "ClockBase.h"

class BasicClock : public ClockBase
{
 public:
  BasicClock();
  void display(SmartMatrix *matrix);
 private:
};
