#ifndef CLOCK_BASE_H
#define CLOCK_BASE_H

#include <cstring>
#include <core_pins.h> // defines teensytime
#include "Time.h"

#include <SmartMatrix.h>

extern void SetFromCompilerTime(int offset);

class ClockBase
{
 public:
  ClockBase();
  virtual ~ClockBase() {}
  
  friend void SetFromCompiledTime(int offset);
  
  virtual void display(SmartMatrix *matrix) = 0;
  
  bool update(); // returns true if the time has changed
  
  void serialPrint();
	
 protected:
 
  // Flag for 24 or 12 hour time
  
  
  void getHour(char*);   // expects char[3]
  void getMinute(char*); // expects char[3]
  
  //void getMonth(char[2]);
  //void DisplayDay();
 
  bool useAmPm;
  time_t time;
  
  void printNumToString(int num, char *ptr);
};

#endif // CLOCK_BASE_H
