#ifndef DISPLAY_MAX_DECAY_H
#define DISPLAY_MAX_DECAY_H

#include "DisplayBase.h"
#include "DecayClasses.h"
#include "ArraySharedPtr.h"
#include "Compression.h"

#include <limits>

class DisplayLimitDecay : public DisplayBase
{
 public:
  enum Type
  {
    Max,
    Min
  };
  // TODO Define Max Y, X, direction?
  
  DisplayLimitDecay(Type type = Max);
  DisplayLimitDecay(const DisplayLimitDecay &rhs);
  ~DisplayLimitDecay();
  
  void setDecayFunction(const ArraySharedPtr<DecayFunciton> &array);
  
  void display(DataBuffer &data, SmartMatrix *matrix);

 private:
  DataBuffer m_LimitBuffer;
  uint16_t m_time[128];
  ArraySharedPtr<DecayFunciton> m_decayfunctionArray;
  Type m_type;
};

#endif // DISPLAY_MAX_DECAY_H
