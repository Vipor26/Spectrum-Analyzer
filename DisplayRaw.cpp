#include "DisplayRaw.h"

void DisplayRaw::display(DataBuffer &data, SmartMatrix *matrix)
{
  uint16_t i,s;
  
  s = data.size;
  for( i=0; i<s; ++i)
  {
    // in range chack is already done by the matrix display
    matrix->drawPixel(data.data[i].X,data.data[i].Y,data.data[i].C);
  }
}
