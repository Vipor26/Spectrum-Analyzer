#ifndef REMAP_LINEAR_H
#define REMAP_LINEAR_H

#include "RemapBase.h"
#include "Compression.h"

class RemapLinear : public RemapBase, private Compression
{
 public:
 
  enum LinearMapping
  {
    ScaleX,
    ScaleY,
    ScaleXY
  };
 
  RemapLinear( LinearMapping type, 
               uint16_t HorzSize,
               uint16_t VertSize,
               Compression::Type comp );
               
  RemapLinear( const RemapLinear &rhs);
 
  void remap(DataBuffer &data);
 
  void lockHorz(double val);
  void lockVert(double val);
 
 private:
  LinearMapping mapping;
  double scaleX, scaleY;
  uint16_t horzSize, vertSize;
  
  void rescaleX(DataBuffer &data);
  void rescaleY(DataBuffer &data);
};

#endif
