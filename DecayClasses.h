#ifndef DECAY_CLASSES_H
#define DECAY_CLASSES_H

#include "RemapBase.h"

// ----- Decay function base ----
class DecayFunciton
{
 public:
  // if false is returned all the points after this in the queue are dropped
  virtual void apply(Data &data, uint16_t time) = 0;
};

// ---- Decay function that mimics gravity ----
class DecayGravity : public DecayFunciton
{
 public:
  DecayGravity(double Gravity, double TimeScale);
  
  void apply(Data &data, uint16_t time);
  
 private:
  double gravity, timeScale;
};

// ---- Fade function ----
class DecayFade : public DecayFunciton
{
 public:
  DecayFade(double fadeConst);
  
  void apply(Data &data, uint16_t time);
  
 private:
  double FadeConst;
};

#endif // DECAY_CLASSES_H
