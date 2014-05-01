#ifndef PERSISTANT_DISPLAY_H
#define PERSISTANT_DISPLAY_H

#include "CircularBufferCT.h"
#include "FFT_DisplayBase.h"
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
class GravityDecay : public DecayFunciton
{
 public:
  GravityDecay(double Gravity, double TimeScale);
  
  void apply(uint16_t &data, uint16_t time);
  
 private:
  double gravity, timeScale;
};

// ---- Fade function ----
class GravityDecay : public DecayFunciton
{
 public:
  GravityDecay(double Gravity, double TimeScale);
  
  void apply(uint16_t &data, uint16_t time);
  
 private:
  double gravity, timeScale;
};


//==== Define Persistant display ====
class PersistantDisplay : public FFT_DisplayBase
{
 public:
  void setFadeFunction(std::shared_ptr<DecayFunction> &DecayFunction);
  //void setPainter();
  void display(uint16_t *FFT_Data, SmartMatrix *matrix);

 private:
  // want to make this clean so opted for a pound define instead of template
  CircularBuffer<uint16_t,PERSISTANT_BUFFER_SIZE> buffers[128];
  std::shared_ptr<DecayFunction> *decayFunction;
};

#endif

void PersistantDisplay::setFadeFunction(std::shared_ptr<DecayFunction> &DecayFunction)
{
  decayFunction = DecayFunction;
}
