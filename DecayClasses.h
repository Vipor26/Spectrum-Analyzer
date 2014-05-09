#ifndef DECAY_CLASSES_H
#define DECAY_CLASSES_H

#include "RemapBase.h"

#include <SmartMatrix.h>

// ----- Decay function base ----
class DecayFunciton
{
 public:
  // if true is returned all the points after this in the queue are dropped
  virtual bool apply(Data &data, uint16_t time) = 0;
  
};

// ---- Decay function that mimics gravity ----
class DecayGravity : public DecayFunciton
{
 public:
  DecayGravity(double Gravity, double TimeScale, uint16_t hold);
  
  bool apply(Data &data, uint16_t time);
  
 private:
  double m_gravity, m_timeScale;
  uint16_t m_hold; // time to hold for
};

// ---- Fade function ----
class DecayFade : public DecayFunciton
{
 public:
  DecayFade(uint8_t fadeConst); // amount to decrease intensity 1-255
  
  bool apply(Data &data, uint16_t time);
  
 private:
  void apply(uint8_t &comp);
 
  uint8_t m_fadeConst;
};

#endif // DECAY_CLASSES_H
