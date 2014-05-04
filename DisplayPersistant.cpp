#include "DisplayPersistant.h"


DisplayPersistant::DisplayPersistant()
{  }

DisplayPersistant::DisplayPersistant(const DisplayPersistant &rhs) : m_decayFunction(rhs.m_decayFunction)
{
  for(uint16_t index=0; index<128; ++index)
  {
    buffers[index] = rhs.buffers[index];
  }
}

DisplayPersistant::~DisplayPersistant()
{  }

void DisplayPersistant::setFadeFunction(std::shared_ptr<DecayFunciton> &decayFunction)
{
  m_decayFunction = decayFunction;
}

void DisplayPersistant::display(DataBuffer &data, SmartMatrix *matrix)
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
