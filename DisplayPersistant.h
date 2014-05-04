#ifndef PERSISTANT_DISPLAY_H
#define PERSISTANT_DISPLAY_H

#include "CircularBufferCT.h"
#include "DisplayBase.h"
#include "DecayClasses.h"
#include <memory>

#ifndef defined(PERSISTANT_BUFFER_SIZE)
#define PERSISTANT_BUFFER_SIZE 4
#endif


class DisplayPersistant : public DisplayBase
{
 public:
 DisplayPersistant();
 DisplayPersistant(const DisplayPersistant &rhs);
 ~DisplayPersistant();
 
 void setFadeFunction(std::shared_ptr<DecayFunciton> &decayFunction);
 //void setPainter();
 void display(DataBuffer &data, SmartMatrix *matrix);

 private:
  // want to make this clean so opted for a pound define instead of template
  CircularBuffer<uint16_t,PERSISTANT_BUFFER_SIZE> buffers[128];
  std::shared_ptr<DecayFunciton> m_decayFunction;
};

#endif
