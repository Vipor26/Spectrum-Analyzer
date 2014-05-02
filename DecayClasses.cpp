#include "DecayClasses.h"

// ---- Decay function that mimics gravity ----
DecayGravity::DecayGravity(double Gravity, double TimeScale):
  timeScale(TimeScale),
  gravity(Gravity)
{  }

void DecayGravity::apply(uint16_t &data, uint16_t time)
{
  const double t = (timeScale*time);
  data = 0.5*gravity*t*t;
}

// ---- Fade function ----
DecayFade::DecayFade(double fadeConst)
{
  
}
  
void DecayFade::apply(uint16_t &data, uint16_t time)
{
  
}
