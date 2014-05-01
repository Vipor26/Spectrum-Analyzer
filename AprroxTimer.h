#ifndef APRROX_TIMER
#define APRROX_TIMER

class AprroxTimer
{
 public:
  AprroxTimer(uint16_t Seconds) :
    offset(Seconds),
    lastTime(millis())
  {  }
 
  bool hit(const unsigned int &time)
  {
    return ( lastTime + offset <= time );
  }
 
  unsigned int diffms(const unsigned int &time) const
  {
    // This if case should never happen
    if(lastTime < time)  {
      return (time - lastTime);
    }
    return (lastTime - time);
  }
 
  void reset(const unsigned int &time)
  {
    lastTime = time;
  }
  
 private:
  unsigned int lastTime;
  const uint16_t offset;
};

#endif
