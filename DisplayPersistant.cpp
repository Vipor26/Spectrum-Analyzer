#include "DisplayPersistant.h"

// ---- Decay function that mimics gravity ----
DecayGravity::DecayGravity(double Gravity, double TimeScale):
  timeScale(TimeScale),
  gravity(Gravity)
{  }

void DecayGravity::apply(uint16_t &data, uint16_t time)
{
  const double t = (timeScale*time);
  data = 0.5*gravity*t*t;
}

//==== Define Persistant display ====

void DisplayPersistant::setFadeFunction(std::shared_ptr<DecayFunciton> &decayFunction)
{
  m_decayFunction = decayFunction;
}

void DisplayPersistant::display(uint16_t *FFT_Data, SmartMatrix *matrix)
{
  // Apply decay functions and dump whatever is not needed
  uint8_t index, bufIndex, bufSize;
  for(index=0; index<128; +index)
  {
    bufSize = buffers[index].size();
    for(bufIndex=0; bufIndex<bufSize; ++bufIndex)
    {
      //decayFunction(buffers[index][bufIndex],bufIndex);
    }
  }
  
  //Display data
}
