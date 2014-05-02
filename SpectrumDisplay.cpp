#include "SpectrumDisplay.h"

void SpectrumDisplay::display(DataBuffer &data, SmartMatrix *matrix)
{
  uint8_t index,size;
  
  //Remap the data
  size = m_remappers.size();
  for(index=0; index<size; ++index)
  {
    m_remappers[index]->remap(data);
  }
  
  //generate display data
  matrix->fillScreen({0x00,0x00,0x00});
  matrix->setRotation(rotation180);
  
  for(index=0; index<size; ++index)
  {
    m_displayers[index]->display(data, matrix);
  }
  
  matrix->swapBuffers(true);
}
