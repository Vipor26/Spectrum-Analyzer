

#ifndef DISPLAY_RAW_H
#define DISPLAY_RAW_H

#include "DisplayBase.h"

class DisplayRaw : DisplayBase
{
 public:
  
  void display(DataBuffer &data, SmartMatrix *matrix);
  
};

#endif // DISPLAY_RAW_H
