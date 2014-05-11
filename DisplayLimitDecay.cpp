#include "DisplayLimitDecay.h"

DisplayLimitDecay::DisplayLimitDecay(Type type) : m_type(type)
{
  uint16_t point;
  switch(m_type)
  {
    case Max:
      point = 0;
      break;
    case Min:
      point = std::numeric_limits<uint16_t>::max();
      break;
  }
  
  for(uint16_t index=0; index<128; ++index)
  {
    m_LimitBuffer.data[index].Y = point;
      
    m_time[index] = 0;
    m_LimitBuffer.size = 128;
  }
}

DisplayLimitDecay::DisplayLimitDecay(const DisplayLimitDecay &rhs) :
  m_decayfunctionArray(rhs.m_decayfunctionArray),
  m_type(rhs.m_type)
{
  for(uint16_t index=0; index<128; ++index)
  {
    m_LimitBuffer.data[index] = rhs.m_LimitBuffer.data[index];
    m_time[index] = rhs.m_time[index];
  }
}
  
DisplayLimitDecay::~DisplayLimitDecay()
{  }


void DisplayLimitDecay::setDecayFunction(const ArraySharedPtr<DecayFunciton> &array)
{
  m_decayfunctionArray = array;
}
  
void DisplayLimitDecay::display(DataBuffer &data, SmartMatrix *matrix)
{
  uint16_t index, decayIndex, dataSize, x, y, h;
  
  //First set decay on max's
  dataSize = data.size;
  for(index=0; index<dataSize; ++index)
  {
    if(m_LimitBuffer.data[index].Y != 0)
    {
      for(decayIndex=0; decayIndex<m_decayfunctionArray.size(); ++decayIndex)
      {
        if( m_decayfunctionArray[decayIndex] == NULL )  {
          continue;
        }
        if( !m_decayfunctionArray[decayIndex]->apply( m_LimitBuffer.data[index], m_time[index]) )
        {
          // decay limit not reached and still drawable
          // anything to do here?
        }
      }
    }
  }
  
  //Second do max text
  
  
  switch(m_type)
  {
    case Max:
      for(index=0; index<dataSize; ++index)
      {
        if(data.data[index].Y > m_LimitBuffer.data[index].Y)
        {
          m_LimitBuffer.data[index] = data.data[index];
          m_time[index] = 0;
        }
        else  {
          m_time[index]++;
        }
      }
      break;
    case Min:
      for(index=0; index<dataSize; ++index)
      {
        if(data.data[index].Y < m_LimitBuffer.data[index].Y)
        {
          m_LimitBuffer.data[index] = data.data[index];
          m_time[index] = 0;
        }
        else  {
          m_time[index]++;
        }
      }
      break;
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
  
  //dataSize = m_LimitBuffer.size;
  for( index=0; index<dataSize; ++index)
  {
    x = m_LimitBuffer.data[index].X;
    y = m_LimitBuffer.data[index].Y;
    
    // in range chack is already done by the matrix display
    matrix->drawPixel(x,h-y-1,m_LimitBuffer.data[index].C);
  }
}
