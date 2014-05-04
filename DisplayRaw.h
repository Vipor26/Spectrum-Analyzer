

#ifndef DISPLAY_RAW_H
#define DISPLAY_RAW_H

#include "DisplayBase.h"

class DisplayRaw : public DisplayBase
{
 public:
  DisplayRaw();
  DisplayRaw(const DisplayRaw &rhs);
  ~DisplayRaw();
  
  void display(DataBuffer &data, SmartMatrix *matrix);
};

#endif // DISPLAY_RAW_H
