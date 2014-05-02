#ifndef REMAP_DECIBEL_H
#define REMAP_DECIBEL_H

#include "RemapBase.h"

class RemapDecibel : public RemapBase
{
 public:
 
  void remap(DataBuffer &data);
  
 private:
  uint8_t m_size;
};

#endif
