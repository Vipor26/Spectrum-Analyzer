#ifndef REMAP_DECIBEL_H
#define REMAP_DECIBEL_H

#include "RemapBase.h"

class RemapDecibel : public RemapBase
{
 public:
  RemapDecibel(MatrixSize size);
  void remap(DataBuffer &data);
  
  void lockVert(double scale) { scaleY = scale; }
  
 private:
  double scaleY;
  double tempYBuffer[128];
  uint16_t height;
};

#endif
