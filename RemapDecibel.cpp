#include "Remapdecibel.h"

void RemapDecibel::remap(DataBuffer &data)
{
  uint8_t index, size;
  for(index=0; index<size; ++index)
  {
    data.data[index].Y = log(data.data[index].Y + 1);
  }
}
