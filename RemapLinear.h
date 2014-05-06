#ifndef REMAP_LINEAR_H
#define REMAP_LINEAR_H

#include "BasicStructures.h"
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
               MatrixSize ScreenSize,
               Compression::Type comp );
               
  RemapLinear( const RemapLinear &rhs);
  
  ~RemapLinear() {}
 
  void remap(DataBuffer &data);
 
  void lockHorz(double val);
  void lockVert(double val);
 
 private:
  LinearMapping mapping;
  double scaleX, scaleY;
  MatrixSize screenSize;
  
  void rescaleX(DataBuffer &data);
  void rescaleY(DataBuffer &data);
};

#endif
