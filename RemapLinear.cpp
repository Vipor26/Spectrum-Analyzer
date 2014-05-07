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
    case ScaleXY: rescaleX(data); // no break;
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
  uint16_t index, start, end, where, CurrentX, tempX;
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
  
  start = 0;
  end = 0;
  where = 0;
  CurrentX = data.data[0].X;
  for(index=0; index<data.size; ++index)
  {
    tempX = data.data[index].X;
    if(CurrentX == tempX) {
      continue;
    }
    end = index-1;
    // if this fails then an end was found
    // index-1
    
    //Combiner
    applyComp(start,
              end,
              where,
              data  ); 
    start = index;
    ++where;
    CurrentX = tempX;
  }
  end = data.size-1;
  if(end > start)
  {
    //Combiner
    applyComp(start,
              end,
              where,
              data  ); 
  }
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
