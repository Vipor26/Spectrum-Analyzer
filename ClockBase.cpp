#include "ClockBase.h"

extern time_t now();

long int getTeensy3Time()
{
  return Teensy3Clock.get();
}

ClockBase::ClockBase() : useAmPm(false)
{
  // set the Time library to use Teensy 3.0's RTC to keep time
  setSyncProvider(getTeensy3Time);
  // now that the time library knows how to talk to the RTC clock, update it.
  time = now();
}

void ClockBase::getHour(char *timeStr)
{
  printNumToString( useAmPm ? hourFormat12(time) : hour(time), timeStr);
  timeStr[2] = NULL;
  
}

void ClockBase::getMinute(char *timeStr)
{
  printNumToString(minute(time),timeStr);
  timeStr[2] = NULL;
}

void ClockBase::printNumToString(int num, char *ptr)
{
  int small;
  if(num < 10)  {
    ptr[1] = '0' + (char)num;
    ptr[0] = ' ';
  }
  else  {
    small = num%10;
    num /= 10;
    ptr[1] = '0' + (char)small;
    ptr[0] = '0' + (char)num;
  }
}

bool ClockBase::update()
{
  time_t newTime = now();
  
  if( newTime == time)
  {
    return false;
  }
  time = newTime;
  return true;
}

void ClockBase::serialPrint()
{
  Serial.print(hour(time));
  Serial.print(":");
  Serial.print(minute(time));
  Serial.print(":");
  Serial.print(second(time));
  Serial.print(" ");
  Serial.print(month(time));
  Serial.print("/");
  Serial.print(day(time));
  Serial.print(" ");
  Serial.print(year(time));
}



void SetFromCompilerTime(int offset)
{
  // Just in case this is called before a time object has been instatiated
  // set the Time library to use Teensy 3.0's RTC to keep time
  setSyncProvider(getTeensy3Time);
    
  const char time[] = __TIME__; //"hh:mm:ss"
  const char date[] = __DATE__; //"Mmm dd yyyy"
  
  const char monStr[4] = { date[0], date[1], date[2], NULL };
  
  TimeElements tempTime;
  
  tempTime.Month = 0;
  if(      strcmp(monStr,"Jan") == 0 ) tempTime.Month = 1;
  else if( strcmp(monStr,"Feb") == 0 ) tempTime.Month = 2;
  else if( strcmp(monStr,"Mar") == 0 ) tempTime.Month = 3;
  else if( strcmp(monStr,"Apr") == 0 ) tempTime.Month = 4;
  else if( strcmp(monStr,"May") == 0 ) tempTime.Month = 5;
  else if( strcmp(monStr,"Jun") == 0 ) tempTime.Month = 6;
  else if( strcmp(monStr,"Jul") == 0 ) tempTime.Month = 7;
  else if( strcmp(monStr,"Aug") == 0 ) tempTime.Month = 8;
  else if( strcmp(monStr,"Sep") == 0 ) tempTime.Month = 9;
  else if( strcmp(monStr,"Oct") == 0 ) tempTime.Month = 10;
  else if( strcmp(monStr,"Nov") == 0 ) tempTime.Month = 11;
  else if( strcmp(monStr,"Dec") == 0 ) tempTime.Month = 12;
  
  tempTime.Second = ((int)time[6]-(int)'0')*10 + ((int)time[7]-(int)'0');
  tempTime.Minute = ((int)time[3]-(int)'0')*10 + ((int)time[4]-(int)'0'); 
  tempTime.Hour = ((int)time[0]-(int)'0')*10 + ((int)time[1]-(int)'0');
  tempTime.Day =   ((int)date[4]-(int)'0')*10 + ((int)date[5]-(int)'0');
  int parYear;
  parYear  = ((int)date[7]-(int)'0')*1000;
  parYear += ((int)date[8]-(int)'0')*100;
  parYear += ((int)date[9]-(int)'0')*10;
  parYear += ((int)date[10]-(int)'0');   
  tempTime.Year = parYear;
  
  tempTime.Second += offset;
  int temp = tempTime.Second / 60;
  tempTime.Minute += temp;
  tempTime.Second -= temp*60;
  
  setTime(tempTime.Hour,
          tempTime.Minute,
          tempTime.Second,
          tempTime.Day,
          tempTime.Month,
          tempTime.Year  );
}

void SerialPrintTime()
{
  time_t tempTime = now();
  Serial.print(hour(tempTime));
  Serial.print(":");
  Serial.print(minute(tempTime));
  Serial.print(":");
  Serial.print(second(tempTime));
  Serial.print(" ");
  Serial.print(month(tempTime));
  Serial.print("/");
  Serial.print(day(tempTime));
  Serial.print(" ");
  Serial.print(year(tempTime));
}
