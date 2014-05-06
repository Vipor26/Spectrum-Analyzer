#include "Remapdecibel.h"


RemapDecibel::RemapDecibel(MatrixSize size) : height(size.height), scaleY(0.0)
{  }

// Have to place data in a teperaty place with more
//   precission to eliminate quintization error

// wiki answer, my answer is a little diffrent
// amplitude = 14731 / 32767
//           = 0.44
// dB = 20 * log10(0.44)
//    = -7.13

void RemapDecibel::remap(DataBuffer &data)
{
  uint16_t index;
  double maxY = 0, temp, scale;
  
  const uint16_t size = data.size;
  
  // Apply formula, look for max
  
  for(index=0; index<size; ++index)
  {
    temp = log((double)(data.data[index].Y) + 1.0);
    
    tempYBuffer[index] = temp;
    
    if(temp > maxY)  {
      maxY = temp;
    }
  }
  
  // Calculate scale if scale is not locked
  if(scaleY != 0.0)  {
    scale = scaleY;
  }
  else  {
    scale = ((double)height)/maxY;
  }
  
  // Rescale and place data back into buffer
  for(index=0; index<size; ++index)
  {
    data.data[index].Y = (uint16_t)(tempYBuffer[index]*scale);
  }
}

