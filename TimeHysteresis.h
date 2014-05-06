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
    dir(0),
    notifiedStateChange(NotifiedStateChange),
    lastTime(millis())
  {  }
  
  bool test(const double &value,const unsigned long &time)
  {
    bool testState = (value >= threshold);
    
    int deltaTime = time - lastTime;
    lastTime = time;
    
    if(testState)
    {
      sum += deltaTime;
    }
    else
    {
      sum -= deltaTime;
    }
    
    /*
    if(dir==0)
    {
      if(testState)
        dir=1;
       else  {
        dir=-1;
       }
    }
    
    // if currently true but false was the result of test
    if(       state && !testState)
    {
      dir = -1;
    }
    // if currently false but true was the result of test
    else if( !state && testState )
    {
      dir = 1;
    sum += (deltaTime*dir);
      Serial.print(dir);
      Serial.println();
    }*/
    
    
    
    //else // they agree
    //{
    //  return state;
    //}
    
    if(        sum > trueTime)
    {
      sum = 0;
      if(state == true)  {
        return true;
      }
      
      state = true;
      if(notifiedStateChange != NULL)
      {
        notifiedStateChange(true);
      }
    }
    else if(-1*sum > ((int)falseTime))
    {
      sum = 0;
      if(state == false)  {
        return false;
      }
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
  int sum;
  int dir;
  bool state;
};


#endif

