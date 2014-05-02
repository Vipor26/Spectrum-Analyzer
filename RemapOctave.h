#ifndef REMAP_OCTAVE_H
#define REMAP_OCTAVE_H

#include "RemapBase.h"

class RemapOctave : public RemapBase
{
 public:
  enum OctaveCompression
  {
    OctaveAverage,
    OctaveAll,
    OctaveMax
  };
  
  RemapOctave(uint8_t ReMapTo, OctaveCompression comp); // expects 16 or 32
 
  void remap(DataBuffer &data);
 
 private:
   OctaveCompression compression;
   uint8_t m_size, *m_dataptr;
   
   void useAverage(const uint8_t &start,
                   const uint8_t &stop,
                   const uint8_t &where,
                   DataBuffer &data);
                
   void useAll(const uint8_t &start,
               const uint8_t &stop,
               const uint8_t &where,
               DataBuffer &data);
            
   void useMax(const uint8_t &start,
               const uint8_t &stop,
               const uint8_t &where,
               DataBuffer &data);
};


#endif
