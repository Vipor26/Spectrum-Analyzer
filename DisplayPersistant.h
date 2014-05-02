#ifndef PERSISTANT_DISPLAY_H
#define PERSISTANT_DISPLAY_H

#include "CircularBufferCT.h"
#include "DisplayBase.h"
#include <memory>

#ifndef defined(PERSISTANT_BUFFER_SIZE)
#define PERSISTANT_BUFFER_SIZE 4
#endif

struct PersistantPointData
{
  uint8_t X, Y;
  rgb24 color;
};

// ===== Define Max decay finctions ====
// ----- Decay function base ----
class DecayFunciton
{
 public:
  // if false is returned all the points after this in the queue are dropped
  virtual bool apply( uint16_t &X, uint16_t &Y, uint16_t position) = 0;
};

// ---- Decay function that mimics gravity ----
class DecayGravity : public DecayFunciton
{
 public:
  DecayGravity(double Gravity, double TimeScale);
  
  void apply(uint16_t &data, uint16_t time);
  
 private:
  double gravity, timeScale;
};

// ---- Fade function ----
class DecayFade : public DecayFunciton
{
 public:
  DecayFade(double fadeConst);
  
  void apply(uint16_t &data, uint16_t time);
  
 private:
  double FadeConst;
};


//==== Define Persistant display ====
class DisplayPersistant : public DisplayBase
{
 public:
  void setFadeFunction(std::shared_ptr<DecayFunciton> &decayFunction);
  //void setPainter();
  void display(DataBuffer &data, SmartMatrix *matrix);

 private:
  // want to make this clean so opted for a pound define instead of template
  CircularBuffer<uint16_t,PERSISTANT_BUFFER_SIZE> buffers[128];
  std::shared_ptr<DecayFunciton> m_decayFunction;
};

#endif
