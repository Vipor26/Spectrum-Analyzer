#ifndef COMPRESSOR_H
#define COMPRESSOR_H

#include "RemapBase.h"

class Compression
{
 public:
  enum Type
  {
    Average,
    All,
    Max
  };
  
  Compression(Type type) : m_type(type)
  {  }
  
  void applyComp( const uint8_t &start,
                  const uint8_t &stop,
                  const uint8_t &where,
                  DataBuffer &data     );
   
 private:
   Type m_type; 

   void useAverage( const uint8_t &start,
                    const uint8_t &stop,
                    const uint8_t &where,
                    DataBuffer &data      );

   void useAll( const uint8_t &start,
                const uint8_t &stop,
                const uint8_t &where,
                DataBuffer &data      );

   void useMax( const uint8_t &start,
                const uint8_t &stop,
                const uint8_t &where,
                DataBuffer &data      );
};
                    
#endif // COMPRESSOR_H
