#include "DecayClasses.h"

// ---- Decay function that mimics gravity ----
DecayGravity::DecayGravity(double Gravity, double TimeScale, uint16_t hold):
  m_timeScale(TimeScale),
  m_gravity(Gravity),
  m_hold(hold)
{  }

bool DecayGravity::apply(Data &data, uint16_t time)
{
  
  // if the hold period is not complete
  if(time < m_hold)
  {
    return false;
  }
  
  const double tSec = (m_timeScale*(time - m_hold));
  // tSec = how long the object has been falling for
  
  
  double deltaD = m_gravity*tSec;
  if( data.Y > (uint16_t)deltaD)  {
    data.Y -= deltaD; 
    return false;
  }
  data.Y = 0;
  return true;
}

// ---- Fade function ----
DecayFade::DecayFade(uint8_t fadeConst) : m_fadeConst(fadeConst)
{  }
  
bool DecayFade::apply(Data &data, uint16_t time)
{
  apply(data.C.red);
  apply(data.C.green);
  apply(data.C.blue);
  
  if( (data.C.red   == 0) &&
      (data.C.green == 0) &&
      (data.C.blue  == 0)    )
   {
     return true;
   }
   return false;
}

void DecayFade::apply(uint8_t &comp)
{
  //TODO: have option for not just linear decay?
  if(comp > m_fadeConst)  {
    comp -= m_fadeConst;
  }
  else  {
    comp = 0;
  }
}
