#include "DisplayBase.h"
#include "DecayClasses.h"

class DisplayMaxDecay : public DisplayBase
{
 public:
  DisplayMaxDecay();
  
  void setDecayFunction();
  void setPainter();
  
  void display(DataBuffer &data, SmartMatrix *matrix);

 private:
  DataBuffer MaxBuffer;
};
