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
  
  
  
  if(!m_debugOutLock)
  {
    bool error = false;
    size = data.size;
    for(index=0; index<size; ++index)
    {
      if(data.data[index].X > matrix->getScreenWidth() )
      {
        Serial.print("X out of bounds: ");
        Serial.println(data.data[index].X);
        error = true;
      }
      if(data.data[index].Y > matrix->getScreenHeight() )
      {
        Serial.print("Y out of bounds: ");
        Serial.println(data.data[index].Y);
        error = true;
      }
    }
    
    Serial.println(size);
    m_debugOutLock = true;
    
    //m_debugOutLock = error;
  }
  
  
  size = m_displayers.size();
  for(index=0; index<size; ++index)
  {
    if(m_displayers[index] != NULL)  {
      m_displayers[index]->display(data, matrix);
    }
  }
  
  matrix->swapBuffers(true);
}
