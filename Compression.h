#ifndef COMPRESSOR_H
#define COMPRESSOR_H

#include "RemapBase.h"

class Compression
{
 public:
  enum Type
  {
    Avg,
    All,
    Max,
    None
  };
  
  //enum WhichType
  //{
  //  X,
  //  Y,
  //};
  
  Compression(Type type) : m_type(type)
  {  }
  
  Compression( const Compression &rhs) : m_type(rhs.m_type)
  {  }
  
  // start and stop indexes are included in the calc
  void applyComp( const uint8_t &start,
                  const uint8_t &stop,
                  const uint8_t &where,
                  DataBuffer &data     );
   
  void searchComp( DataBuffer &data );
   
 Type getType() const { return m_type; }
   
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
