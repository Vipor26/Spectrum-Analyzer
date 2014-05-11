#include "DisplayPersistant.h"


DisplayPersistant::DisplayPersistant() :
  m_decayfunctionArray()
{  }

DisplayPersistant::DisplayPersistant(const DisplayPersistant &rhs) :
  m_decayfunctionArray(rhs.m_decayfunctionArray)
{
  for(uint16_t index=0; index<128; ++index)
  {
    m_buffers[index] = rhs.m_buffers[index];
  }
}

DisplayPersistant::~DisplayPersistant()
{  }

void DisplayPersistant::setDecayFunction(const ArraySharedPtr<DecayFunciton> &array)
{
  m_decayfunctionArray = array;
}

void DisplayPersistant::display(DataBuffer &data, SmartMatrix *matrix)
{
  
  // Apply decay functions and dump whatever is not needed
  uint16_t index, size, bufSize, decIndex, x, y, h;
  int bufIndex;
  // Update the decay in the buffers
  size = data.size;
  for(index=0; index<size; ++index)
  {
    // for every pixel in the buffer for this data position
    bufSize = m_buffers[index].size();
    for(bufIndex=0; bufIndex<bufSize; ++bufIndex)
    {
      // for every decay function
      for(decIndex=0; decIndex< m_decayfunctionArray.size(); ++decIndex)
      {
        if( m_decayfunctionArray[decIndex]->apply( m_buffers[index][bufIndex], bufIndex ) )
        {
          m_buffers[index].dropFrom(bufIndex);
        }
      }
    }
  }
  
  // Push new data to the buffers
  for(index=0; index<size; ++index)
  {
    m_buffers[index].push(data.data[index]);
  }
  
  //Display data
  h = matrix->getScreenHeight();
  for(index=0; index<size; ++index)
  {
    bufSize = m_buffers[index].size()-1;
    for(bufIndex=bufSize; bufIndex>=0; --bufIndex)
    {
      x = m_buffers[index][bufIndex].X;
      y = m_buffers[index][bufIndex].Y;
      
      
      // in range chack is already done by the matrix display
      matrix->drawPixel(x,h-y-1,m_buffers[index][bufIndex].C);
    }
  }
}
