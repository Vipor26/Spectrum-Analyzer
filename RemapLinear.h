#ifndef REMAP_LINEAR_H
#define REMAP_LINEAR_H

#include "RemapBase.h"

class RemapLinear : public RemapBase
{
 public:
  enum LinearMapping
  {
    ScaleX,
    ScaleY,
    ScaleXY
  };
 
  RemapLinear(LinearMapping type, uint8_t HorzSize, uint8_t VertSize);
 
  void remap(DataBuffer &data);
 
  void lockHorz(double val)
  {
    scaleX = val;
  }
  
  void lockVert(double val)
  {
    scaleY = val;
  }
 
 private:
  LinearMapping mapping;
  double scaleX, scaleY;
  uint8_t horzSize, vertSize;
  
  void rescaleX(DataBuffer &data);
  void rescaleY(DataBuffer &data);
};

#endif
