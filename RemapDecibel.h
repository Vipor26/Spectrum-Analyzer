#ifndef REMAP_DECIBEL_H
#define REMAP_DECIBEL_H

#include "RemapBase.h"

class RemapDecibel : public RemapBase
{
 public:
  void remap(DataBlock *data, uint8_t *dataSize);
 private:
  
};

#endif