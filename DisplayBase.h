#ifndef DISPLAY_BASE_H
#define DISPLAY_BASE_H

#include <SmartMatrix.h>
#include "Arduino.h"

#include "RemapBase.h"

class DisplayBase
{
 public:
  DisplayBase();
  virtual ~DisplayBase();
  
  virtual void display(DataBuffer &data, SmartMatrix *matrix) = 0;
  
};

#endif // DISPLAY_BASE_H
