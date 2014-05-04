#include "DisplayBase.h"
#include "DecayClasses.h"

class DisplayMaxDecay : public DisplayBase
{
 public:
  DisplayMaxDecay();
  DisplayMaxDecay(const DisplayMaxDecay &rhs);
  ~DisplayMaxDecay();
  
  void setDecayFunction();
  
  void display(DataBuffer &data, SmartMatrix *matrix);

 private:
  DataBuffer MaxBuffer;
};
