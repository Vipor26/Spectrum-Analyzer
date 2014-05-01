#include "DisplayMaxDecay.h"

DisplayMaxDecay::DisplayMaxDecay()
{
  for(uint8_t index=0; index<128; ++index)
  {
    m_max[index] = 0;
  }
}

void DisplayMaxDecay::setDecayFunction()
{
}

void DisplayMaxDecay::setPainter()
{
}
  
void DisplayMaxDecay::display(uint16_t &output, SmartMatrix *matrix)
{
  uint8_t index;
  //First set decay on max's
  for(index=0; index<128; ++index)
  {
  }
  
  //Second do max text
  for(index=0; index<128; ++index)
  {
  }
  
  //Third and finally display
  for(index=0; index<128; ++index)
  {
  }
  
}
