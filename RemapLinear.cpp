#include "RemapLinear.h"

RemapLinear::RemapLinear(LinearMapping type, uint8_t HorzSize, uint8_t VertSize):
    mapping(type),
    scaleX(0.0),
    scaleY(0.0),
    horzSize(HorzSize),
    vertSize(VertSize)
{  }

void RemapLinear::remap(DataBuffer &data)
{
  switch(mapping)
  {
    case ScaleXY: rescaleX(data); // no break;
    case ScaleY:  rescaleY(data); break;
    case ScaleX:  rescaleX(data);
  }
}

  
void RemapLinear::rescaleX(DataBuffer &data)
{
  uint8_t index;
  double scale;
  if( scaleX != 0.0 )
  {
    scale = scaleX;
  }
  else
  {
    uint16_t max = 0, temp;
    for(index=0; index<data.size; ++index)
    {
      temp = data.data[index].X;
      if(temp > max)  {
        max = temp;
      }
    }
    scale = (double)horzSize/max;
  }
  
  for(index=0; index<data.size; ++index)
  {
    data.data[index].X *= scale;
  }
  
  //Combiner?
}

void RemapLinear::rescaleY(DataBuffer &data)
{
  uint8_t index;
  double scale;
  if( scaleX != 0.0 )
  {
    scale = scaleX;
  }
  else
  {
    uint16_t max = 0, temp;
    for(index=0; index<data.size; ++index)
    {
      temp = data.data[index].Y;
      if(temp > max)  {
        max = temp;
      }
    }
    scale = (double)vertSize/max;
  }
  
  for(index=0; index<data.size; ++index)
  {
    data.data[index].Y *= scale;
  }
}
