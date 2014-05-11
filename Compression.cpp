#include "Compression.h"


void Compression::applyComp( const uint8_t &start,
                             const uint8_t &stop,
                             const uint8_t &where,
                             DataBuffer &data      )
{
   switch(m_type)
    {
      case Avg: useAverage(start, stop, where, data); break;
      case All:     useAll(start, stop, where, data); break;
      case Max:     useMax(start, stop, where, data); break;
      case None: break;
    }
}

void Compression::useAverage( const uint8_t &start,
                              const uint8_t &stop,
                              const uint8_t &where,
                              DataBuffer &data      )
{
  uint8_t indexFrom;
  rgb24 color;
  uint16_t count = (uint16_t)stop + 1 - start;
  uint16_t Rsum = 0, Bsum = 0, Gsum = 0;
  unsigned long sum = 0;
  for(indexFrom=start; indexFrom<=stop; ++indexFrom)
  {
    sum += data.data[indexFrom].Y;
    color = data.data[indexFrom].C;
    Rsum += color.red;
    Gsum += color.green;
    Bsum += color.blue;
  }
  color.red   = (uint8_t)(Rsum/count);
  color.green = (uint8_t)(Gsum/count);
  color.blue  = (uint8_t)(Bsum/count);
  data.data[where].X = where;
  data.data[where].Y = (uint16_t)(sum/count);
  data.data[where].C = color;
}
                
void Compression::useAll( const uint8_t &start,
                          const uint8_t &stop,
                          const uint8_t &where,
                          DataBuffer &data      )
{
  uint8_t indexFrom;
  for(indexFrom=start; indexFrom<=stop; ++indexFrom)
  {
    // Just reassign the X axis
    data.data[indexFrom].X = where;
  }
}

void Compression::useMax( const uint8_t &start,
                          const uint8_t &stop,
                          const uint8_t &where,
                          DataBuffer &data      )
{
  uint8_t indexFrom;
  uint16_t max, temp;
  rgb24 maxcolor = {0x00,0x00,0x00};
  
  max = 0;
  for(indexFrom=start; indexFrom<=stop; ++indexFrom)
  {
    temp = data.data[indexFrom].Y;
    if(temp > max)  {
      max = temp;
      maxcolor = data.data[indexFrom].C;
    }
  }
  
  data.data[where].X = where;
  data.data[where].Y = max;
  data.data[where].C = maxcolor;
}


void Compression::searchComp( DataBuffer &data )
{
  uint16_t index, start, end, where, CurrentX, tempX;
  
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
  data.size = where + 1; // is this correct ??
  //ERROR data.size not set !!!
}
