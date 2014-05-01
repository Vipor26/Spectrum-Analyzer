#include "MaxDecayDisplay.h"

MaxDecayDisplay::MaxDecayDisplay()
{
  for(uint8_t index=0; index<128; ++index)
  {
    m_max[index] = 0;
  }
}

void MaxDecayDisplay::setDecayFunction()
{
}

void MaxDecayDisplay::setPainter()
{
}
  
void MaxDecayDisplay::display(uint16_t &output, SmartMatrix *matrix)
{
}
