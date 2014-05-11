#ifndef PERSISTANT_DISPLAY_H
#define PERSISTANT_DISPLAY_H

#include "BasicStructures.h"
#include "ArraySharedPtr.h"
#include "CircularBufferCT.h"
#include "DisplayBase.h"
#include "DecayClasses.h"

//#include "arduino.h"


#define PERSISTANT_BUFFER_SIZE 16

class DisplayPersistant : public DisplayBase
{
 public:
  DisplayPersistant();
  DisplayPersistant( const DisplayPersistant &rhs );
  ~DisplayPersistant();
 
  void setDecayFunction(const ArraySharedPtr<DecayFunciton> &array);
 
  void display(DataBuffer &data, SmartMatrix *matrix);

 private:
  // want to make this clean so opted for a pound define instead of template
  CircularBuffer<Data,PERSISTANT_BUFFER_SIZE> m_buffers[128];

  ArraySharedPtr<DecayFunciton> m_decayfunctionArray;
};

#endif
