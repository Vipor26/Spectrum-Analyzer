#include "SpectrumDisplay.h"

void SpectrumDisplay::display(DataBuffer &data, SmartMatrix *matrix)
{
  uint8_t index,size;
  
  //Remap the data
  size = m_remappers.size();
  for(index=0; index<size; ++index)
  {
    if(m_remappers[index] != NULL)  {
      m_remappers[index]->remap(data);
    }
  }
  
  //generate display data
  matrix->fillScreen({0x00,0x00,0x00});
  matrix->setRotation(rotation0);
  
  size = m_displayers.size();
  for(index=0; index<size; ++index)
  {
    if(m_displayers[index] != NULL)  {
      m_displayers[index]->display(data, matrix);
    }
  }
  
  matrix->swapBuffers(true);
}
