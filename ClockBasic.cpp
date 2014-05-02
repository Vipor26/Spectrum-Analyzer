#include "ClockBasic.h"

ClockBasic::ClockBasic() : ClockBase()
{
}

void ClockBasic::display(SmartMatrix *matrix)
{
  char temp[3];

  if(update())
  {
    temp[0] = '0';
    
    matrix->setRotation(rotation0);
    matrix->fillScreen({0x00,0x00,0x00});
    
    matrix->fillRoundRectangle(0,
                               1,
                               matrix->getScreenWidth()-1,
                               matrix->getScreenHeight()-1,
                               3,
                               {0xFF,0x00,0xFF},
                               {0x00,0x00,0x00}            );
    
    //matrix->drawFastVLine(15,1,matrix->getScreenHeight()-1,{0xFF,0x00,0xFF});
    //matrix->drawFastVLine(16,1,matrix->getScreenHeight()-1,{0xFF,0x00,0xFF});
      
    matrix->drawFastVLine(15,5,6,{0xEF,0xCF,0xEF});
    matrix->drawFastVLine(16,5,6,{0xEF,0xCF,0xEF});
    
    matrix->drawFastVLine(15,10,11,{0xEF,0xCF,0xEF});
    matrix->drawFastVLine(16,10,11,{0xEF,0xCF,0xEF});
    
    // Set font type
    matrix->setFont(font8x13); //3x5 5x7 6x10 8x13
    
    // Draw Hours
    getHour(temp);
    if(temp[0] == ' ')
    {
      temp[0] = temp[1]; // replace the space with the number and 
      temp[1] = NULL;
      matrix->drawString( 7, 2,  {0xFF,0xFF,0xFF}, temp);
    }
    else
    {
      matrix->drawString( 0, 2,  {0xFF,0xFF,0xFF}, temp);
    }
    
    // Draw Minutes
    getMinute(temp);
    if(temp[0] == ' ')
    {
      temp[0] = '0';
    }
    matrix->drawString( 16, 2,  {0xFF,0xFF,0xFF}, temp);
    
    matrix->swapBuffers(true);
  }
}
