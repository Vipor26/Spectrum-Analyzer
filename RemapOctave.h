#ifndef REMAP_OCTAVE_H
#define REMAP_OCTAVE_H

#include "RemapBase.h"
#include "Compression.h"

class RemapOctave : public RemapBase, protected Compression
{
 public:
  
  RemapOctave(uint8_t ReMapTo, Compression::Type comp); // expects 16 or 32
 
  void remap(DataBuffer &data);
 
 private:
   uint8_t m_size, *m_dataptr;
};


#endif
