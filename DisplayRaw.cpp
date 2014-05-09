#include "DisplayRaw.h"

DisplayRaw::DisplayRaw()
{  }

DisplayRaw::DisplayRaw(const DisplayRaw &rhs)
{  }

DisplayRaw::~DisplayRaw()
{  }

void DisplayRaw::display(DataBuffer &data, SmartMatrix *matrix)
{
  bool dataOutOfRange = false;
  uint16_t i,s,w,h,y,x, ex, ey;
  ex = 0;
  ey = 0;
  h = matrix->getScreenHeight();
  w = matrix->getScreenWidth();
  s = data.size;
  for( i=0; i<s; ++i)
  {
    x = data.data[i].X;
    y = data.data[i].Y;
    if( (y >= h ) || (x >= w) )
    {
      dataOutOfRange = true;
      if( x > ex ) ex = x;
      if( y > ey ) ey = y; 
      continue;
    } 
    
    // in range chack is already done by the matrix display
    matrix->drawPixel(x,h-y-1,data.data[i].C);
  }
  if(dataOutOfRange)
  {
    //Serial.print("DrawRaw had data out of range of this display (");
    //Serial.print(ex);
    //Serial.print(",");
    //Serial.print(ey);
    //Serial.println(")");
  }
}
