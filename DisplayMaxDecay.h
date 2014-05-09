#ifndef DISPLAY_MAX_DECAY_H
#define DISPLAY_MAX_DECAY_H

#include "DisplayBase.h"
#include "DecayClasses.h"
#include "ArraySharedPtr.h"
#include "Compression.h"

class DisplayMaxDecay : public DisplayBase
{
 public:
  
  // TODO Define Max Y, X, direction?
  
  DisplayMaxDecay();
  DisplayMaxDecay(const DisplayMaxDecay &rhs);
  ~DisplayMaxDecay();
  
  void setDecayFunction(const ArraySharedPtr<DecayFunciton> &array);
  
  void display(DataBuffer &data, SmartMatrix *matrix);

 private:
  DataBuffer m_MaxBuffer;
  uint16_t m_time[128];
  ArraySharedPtr<DecayFunciton> m_decayfunctionArray;
};

#endif // DISPLAY_MAX_DECAY_H
