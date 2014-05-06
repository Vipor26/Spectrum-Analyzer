#ifndef BASIC_STRUCTURES_H
#define BASIC_STRUCTURES_H

#include "arduino.h"
#include "SmartMatrix.h"

struct Data
{
  uint16_t X; 
  uint16_t Y;
  rgb24 C;
};


struct DataBuffer
{
  Data data[128];
  uint8_t size;
};


struct MatrixSize
{
  MatrixSize() : width(0), height(0)
  {  }
  
  MatrixSize(const MatrixSize &rhs) : width(rhs.width), height(rhs.height)
  {  }
  
  MatrixSize(SmartMatrix &rhs):
    width(rhs.getScreenWidth()),
    height(rhs.getScreenHeight())
  {  }
  
  ~MatrixSize()
  {  }
  
  uint16_t width;
  uint16_t height;
};
#endif // BASIC_STRUCTURES_H

