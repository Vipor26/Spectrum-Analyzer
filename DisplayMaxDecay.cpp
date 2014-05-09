#include "DisplayMaxDecay.h"

DisplayMaxDecay::DisplayMaxDecay()
{
  for(uint16_t index=0; index<128; ++index)
  {
    m_MaxBuffer.data[index].Y = 0;
    m_time[index] = 0;
    m_MaxBuffer.size = 128;
  }
}

DisplayMaxDecay::DisplayMaxDecay(const DisplayMaxDecay &rhs) : m_decayfunctionArray(rhs.m_decayfunctionArray)
{
  for(uint16_t index=0; index<128; ++index)
  {
    m_MaxBuffer.data[index] = rhs.m_MaxBuffer.data[index];
    m_time[index] = rhs.m_time[index];
  }
}
  
DisplayMaxDecay::~DisplayMaxDecay()
{  }


void DisplayMaxDecay::setDecayFunction(const ArraySharedPtr<DecayFunciton> &array)
{
  m_decayfunctionArray = array;
}
  
void DisplayMaxDecay::display(DataBuffer &data, SmartMatrix *matrix)
{
  uint16_t index, decayIndex, dataSize, x, y, h;
  
  //First set decay on max's
  dataSize = data.size;
  for(index=0; index<dataSize; ++index)
  {
    if(m_MaxBuffer.data[index].Y != 0)
    {
      for(decayIndex=0; decayIndex<m_decayfunctionArray.size(); ++decayIndex)
      {
        if( m_decayfunctionArray[decayIndex] == NULL )  {
          continue;
        }
        if( !m_decayfunctionArray[decayIndex]->apply( m_MaxBuffer.data[index], m_time[index]) )
        {
          // decay limit not reached and still drawable
          // anything to do here?
        }
      }
    }
  }
  
  //Second do max text
  for(index=0; index<dataSize; ++index)
  {
    if(data.data[index].Y > m_MaxBuffer.data[index].Y)
    {
      m_MaxBuffer.data[index] = data.data[index];
      //m_MaxBuffer.data[index].C = {255,255,255};
      m_time[index] = 0;
    }
    else  {
      m_time[index]++;
    }
  }
  
  
  
  
  //Third and finally display
  h = matrix->getScreenHeight();
  //for( index=0; index<dataSize; ++index)
  //{
  //  x = data.data[index].X;
  //  y = data.data[index].Y;
    
    // in range chack is already done by the matrix display
  //  matrix->drawPixel(x,h-y-1,data.data[index].C);
  //}
  
  //dataSize = m_MaxBuffer.size;
  for( index=0; index<dataSize; ++index)
  {
    x = m_MaxBuffer.data[index].X;
    y = m_MaxBuffer.data[index].Y;
    
    // in range chack is already done by the matrix display
    matrix->drawPixel(x,h-y-1,m_MaxBuffer.data[index].C);
  }
}
