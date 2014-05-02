#include "ClockFull.h"

ClockFull::ClockFull() : ClockBase()
{
}

void ClockFull::display(SmartMatrix *matrix)
{
  char temp[10], *part;

  if(update())
  {
    matrix->setRotation(rotation0);
    matrix->fillScreen({0x00,0x00,0x00});
    
    matrix->setFont(font3x5); //3x5 5x7 6x10 8x13
  
    // Display weekday  
    matrix->drawString( 0, 1, {0xFF,0xFF,0xFF}, dayShortStr(weekday(time)));
    matrix->drawPixel( 12, 5, {0xFF,0xFF,0xFF}); // Period
  
    // Display hour
    printNumToString(hour(time), temp);
    temp[2] = NULL;
    matrix->drawString( 15, 1,  {0xFF,0xFF,0xFF}, temp);
  
    // Draw :
    matrix->drawPixel(23, 2, {0xFF,0xFF,0xFF});
    matrix->drawPixel(23, 4, {0xFF,0xFF,0xFF});
  
    // Display minute
    printNumToString(minute(time), temp);
    if(temp[0] == ' ') temp[0] = '0';
    temp[2] = NULL;
    matrix->drawString( 25, 1,  {0xFF,0xFF,0xFF}, temp);
  
  
    for(int i=1; i< matrix->getScreenWidth()-1; ++i )
    {
      if((i-1) % 5 == 0) continue;
      matrix->drawPixel( i-1,7, {0xA0,0xA0,0xA0});
    }
  
    int secondIndex = second(time);
    if( secondIndex >= 30) secondIndex = 59 - secondIndex;
  
    matrix->drawPixel(      secondIndex + 1, 7, {0x00,0x00,0xA0});
    matrix->drawPixel( 30 - secondIndex - 1, 7, {0xA0,0x00,0x00});  
  
  
    // Display month
    matrix->drawString( 0, 9, {0xFF,0xFF,0xFF}, monthShortStr(month(time)));
    matrix->drawPixel( 12,13, {0xFF,0xFF,0xFF}); // Period
  
    // Display Day
    printNumToString(day(time), temp);
    temp[2] = NULL;
    matrix->drawString( 15, 9,  {0xFF,0xFF,0xFF}, temp);
  
    // Display Day
    printNumToString(day(time), temp);
    temp[2] = NULL;
    matrix->drawString( 15, 9,  {0xFF,0xFF,0xFF}, temp);
  
    // Draw ,
    matrix->drawPixel(22, 14, {0xFF,0xFF,0xFF});
    matrix->drawPixel(23, 13, {0xFF,0xFF,0xFF});
  
    // Display minute
    printNumToString(year(time) - 2000, temp);
    if(temp[0] == ' ') temp[0] = '0';
    temp[2] = NULL;
    matrix->drawString( 25, 9,  {0xFF,0xFF,0xFF}, temp);
    
    matrix->swapBuffers(true); 
  }
}
