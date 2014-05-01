#include "PersistantDisplay.h"

// ---- Decay function that mimics gravity ----
GravityDecay::GravityDecay(double Gravity, double TimeScale):
  timeScale(TimeScale),
  gravity(Gravity)
{  }

void GravityDecay::apply(uint16_t &data, uint16_t time)
{
  const double t = (timeScale*time);
  data = 0.5*gravity*t*t;
}

//==== Define Persistant display ====
void PersistantDisplay::display(uint16_t *FFT_Data, SmartMatrix *matrix)
{
  uint8_t index, bufIndex, bufSize;
  for(index=0; index<128; +index)
  {
    bufSize = buffers[index].size();
    for(bufIndex=0; bufIndex<bufSize; ++bufIndex)
    {
      decayFunction(buffers[index][bufIndex],bufIndex);
    }
  }
}
