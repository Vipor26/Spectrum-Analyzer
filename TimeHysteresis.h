#ifndef TIME_HYSTERESIS
#define TIME_HYSTERESIS

class TimeHysteresis
{
 public:
  TimeHysteresis(bool DefaultState, unsigned int trueTime, unsigned int FalseTime, double Threshold, void (*NotifiedStateChange)(bool) = NULL) :
    state(DefaultState),
    trueTime(trueTime),
    falseTime(FalseTime),
    threshold(Threshold),
    sum(0),
    notifiedStateChange(NotifiedStateChange),
    lastTime(millis())
  {  }
  
  bool test(const double &value,const unsigned long &time)
  {
    bool testState = (value >= threshold);
    
    unsigned long deltaTime = time - lastTime;
    lastTime = time;
    
    // if currently true but false was the result of test
    if(       state && !testState)
    {
      sum -= deltaTime;
    }
    // if currently false but true was the result of test
    else if( !state && testState )
    {
      sum += deltaTime;
    }
    else // they agree
    {
      return state;
    }
    
    if(        sum > trueTime)
    {
      sum = 0;
      state = true;
      if(notifiedStateChange != NULL)
      {
        notifiedStateChange(true);
      }
    }
    else if(-1*sum > falseTime)
    {
      sum = 0;
      state = false;
      if(notifiedStateChange != NULL)
      {
        notifiedStateChange(false);
      }
    }
    
    return state;
  }
  
 private:
  void (*notifiedStateChange)(bool);
  unsigned int trueTime, falseTime;
  double threshold;
  unsigned long lastTime;
  long sum;
  bool state;
};


#endif

