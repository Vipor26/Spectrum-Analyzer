#include "RemapLinear.h"

RemapLinear::RemapLinear( LinearMapping type,
                          MatrixSize ScreenSize,
                          Compression::Type comp) :
    mapping(type),
    scaleX(0.0),
    scaleY(0.0),
    screenSize(ScreenSize),
    Compression(comp)
{  }

RemapLinear::RemapLinear( const RemapLinear &rhs) :
    mapping(rhs.mapping),
    scaleX( rhs.scaleX),
    scaleY( rhs.scaleY),
    screenSize(rhs.screenSize),
    Compression(rhs)
{  }

void RemapLinear::remap(DataBuffer &data)
{
  switch(mapping)
  {
    case ScaleXY: rescaleX(data); // no break
    case ScaleY:  rescaleY(data); break;
    case ScaleX:  rescaleX(data);
  }
}


void RemapLinear::lockHorz(double val)
{
  scaleX = val;
}
  
void RemapLinear::lockVert(double val)
{
  scaleY = val;
}

void RemapLinear::rescaleX(DataBuffer &data)
{
  uint16_t index;

  double scale;
  if( scaleX != 0.0 )
  {
    scale = scaleX;
  }
  else
  {
    // Calculate Scale
    uint16_t max = 0, temp;
    for(index=0; index<data.size; ++index)
    {
      temp = data.data[index].X;
      if(temp > max)  {
        max = temp;
      }
    }
    scale = ((double)(screenSize.width))/max;
  }
  
  
  for(index=0; index<data.size; ++index)
  {
    data.data[index].X *= scale;
  }
  
  searchComp(data);
  

  
}

void RemapLinear::rescaleY(DataBuffer &data)
{
  uint8_t index;
  double scale;
  if( scaleY != 0.0 )
  {
    scale = scaleY;
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
    scale = ((double)screenSize.height)/max;
  }
  
  for(index=0; index<data.size; ++index)
  {
    data.data[index].Y *= scale;
  }
  
  //TODO Combiner?
}
