#include "DisplayRaw.h"

DisplayRaw::DisplayRaw()
{  }

DisplayRaw::DisplayRaw(const DisplayRaw &rhs)
{  }

DisplayRaw::~DisplayRaw()
{  }

void DisplayRaw::display(DataBuffer &data, SmartMatrix *matrix)
{
  uint16_t i,s,h,y,x;
  
  h = matrix->getScreenHeight();
  s = data.size;
  for( i=0; i<s; ++i)
  {
    x = data.data[i].X;
    y = data.data[i].Y;
    if( y >= h ) continue;
    
    // in range chack is already done by the matrix display
    matrix->drawPixel(x,h-y-1,data.data[i].C);
  }
}
