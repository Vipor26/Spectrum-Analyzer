#ifndef BLINK_PATTERN_MANAGER
#define BLINK_PATTERN_MANAGER

#include <vector> // easy array

class BlinkPatternManager
{
 public:
  BlinkPatternManager(std::initializer_list<uint16_t> list, uint16_t Ledpin) :
    delayVector(list),
    ledpin(Ledpin),
    lastTime(millis()),
    flag(true),
    index(0)
  {
    pinMode(ledpin, OUTPUT);
    digitalWrite(ledpin, flag);
  }
  
  void update(const unsigned long &time)
  {
    //if last time + delay is in the past, time to update
    if( delayVector[ index ] + lastTime <= time )
    {
      lastTime = time;
      flag = !flag;
      index = (index+1) % delayVector.size();
      
      digitalWrite(ledpin, flag);
    }
  }
  
 private:
  bool flag;
  uint16_t index;
  const uint16_t ledpin;
  unsigned long lastTime;
  std::vector<uint16_t> delayVector;
  
};

#endif
