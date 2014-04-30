// 



// =================== SPECIAL THANKS TO ==========================

// - Paul Stoffregen for the teensy and audio library
//   Documentation: http://www.pjrc.com/teensy/td_libs_Audio.html
//   Library: https://github.com/PaulStoffregen/Audio

// - Louis Beaudoin for the //matrix library
//   Documentation:
//   Library: 

// - Processor doc: https://www.pjrc.com/teensy/K20P64M72SF1RM.pdf


// TODO:
// analogRead(38) - temperature of teensy?
// the temperature of the crystal will influence time accuracy by
// –0.035 ppm/°C × (T – T0) ±10% where T0 is T0 = 25°C ±5°C
// http://www.ti.com/lit/an/slaa322b/slaa322b.pdf

// ######################## Includes ########################
// Include audio
#include <input_adc.h>
#include <analyze_fft256.h>

// Include Screen
#include <SmartMatrix.h>

// Include definsions for 128 colors in HSV H = 360/128*i
#include "HSV_Color.h" 

// The following are the includes for the clock base class, and all the
//    diffrent derivations (derived) classes for displaying a clock
#include "ClockBase.h" // Base Class
#include "BasicClock.h"
#include "FullClock.h"

// The following are the includes for the fft display base class, and all the
//    diffrent affects that can be placed on top of eachother.
#include "FFT_DisplayBase.h" // Base Class

// The following are the includes for the fft remapp base class, these classes
//    map the input data onto the display for the display classes
#include "FFT_RemapperBase.h" // Base Class

// System includes
#include <memory> // shared pointers !!
#include <vector> // easy array
#include <initializer_list> // look at the blink initalizer :)

// ######################## System Defines ########################
// 3 dec doubling in energy 1 is lowest
// every octive is a doubling in frequency

//---- Display ----
SmartMatrix matrix;
const int INT_SCREEN_DELAY = 1000;
const int DefaultBrightness = 25*(255/100);
rgb24 BLACK = {0,0,0};


//---- Audio  ----
AudioAnalyzeFFT256  myFFT(4,0);//(10); 5
AudioInputAnalog   audioInput(17,DEFAULT);  // Pin 17 = A3, Default = 0-3.3v

AudioConnection Connection(audioInput, myFFT);

//---- Time & Display ----
uint8_t CurrentClock = 1;
std::vector<std::shared_ptr<ClockBase>> Clocks = {NULL,NULL}; //way to get arround initalization problems

//---- Equalizer Display
std::vector<std::shared_ptr<FFT_DisplayBase>> CurrentEqualizerSceen;


//---- Status Led ----
class BlinkPattern
{
 public:
  BlinkPattern(std::initializer_list<uint16_t> list, uint16_t Ledpin) :
    delayVector(list),
    ledpin(Ledpin),
    lastTime(millis()),
    flag(true),
    index(0)
  {
    pinMode(ledpin, OUTPUT);
    digitalWrite(ledpin, flag);
  }
  
  void update(const unsigned long &time)
  {
    //if last time + delay is in the past, time to update
    if( delayVector[ index ] + lastTime <= time )
    {
      lastTime = time;
      flag = !flag;
      index = (index+1) % delayVector.size();
      
      digitalWrite(ledpin, flag);
    }
  }
  
 private:
  bool flag;
  uint16_t index;
  const uint16_t ledpin;
  unsigned long lastTime;
  std::vector<uint16_t> delayVector;
} BlinkStatus({1000,1000,100,1000},13);

//  ######################## System set up ########################
void setup()
{ 
  // ---- Set up audio ----
  AudioMemory(12);

  // ---- Set up the screen with a little test sequence ----
  rgb24 RED   = {255,0,0};
  rgb24 GREEN = {0,255,0};
  rgb24 BLUE  = {0,0,255};
  
  matrix.begin();
  matrix.setBrightness(DefaultBrightness);
  //matrix.setScrollOffsetFromEdge(defaultScrollOffset);
  matrix.setColorCorrection(cc24);
  //audioInput.begin(A4,DEFAULT);
  
  matrix.fillScreen(RED);       //
  matrix.swapBuffers();         // swap buffers
  delay(INT_SCREEN_DELAY);                  // waits for a second
  
  matrix.fillScreen(GREEN);
  matrix.swapBuffers();
  delay(INT_SCREEN_DELAY);                  // waits for a second
  
  matrix.fillScreen(BLUE);
  matrix.swapBuffers();
  delay(INT_SCREEN_DELAY);                  // waits for a second
  
  matrix.fillScreen(BLACK);
  matrix.swapBuffers();
  
  
  // ---- Set up the serial debug ----
  // CTRL SHIFT M
  Serial.begin(115200);
  delay(1000);
  Serial.println("System Initalized");

  // ---- Set the clock ----
  //  Based on when this was compiled.
  SetFromCompilerTime(5 + 3*INT_SCREEN_DELAY); //takes 5 seconds to compile +3 to run to this point
  //  Print what the current time is
  SerialPrintTime();
  
  // ---- Set up clocks
  Clocks[0] = std::make_shared<BasicClock>();
  Clocks[1] = std::make_shared<FullClock>();
  
  // ---- Set up scence builder ----
  // ????
  

}

//  ########################## Main Loop ##########################

//  ---- Function prototypes ----
void updateLED();
void updateClock();
void updateEqualizer();
double RMS();

// ---- Logic global declerations ----


class TimeHysteresis
{
 public:
  TimeHysteresis(bool DefaultState, unsigned int trueTime, unsigned int FalseTime, double Threshold) :
    state(DefaultState),
    trueTime(trueTime),
    falseTime(FalseTime),
    threshold(Threshold),
    sum(0),
    lastTime(millis())
  {
  }
  
  bool test(const double &value,const unsigned long &time)
  {
    bool testState = (value >= threshold);
    
    unsigned long deltaTime = time - lastTime;
    lastTime = time;
    
    // if currently true but false was the result of test
    if(       state && !testState)
    {
      sum -= deltaTime;
    }
    // if currently false but true was the result of test
    else if( !state && testState )
    {
      sum += deltaTime;
    }
    else // they agree
    {
      return state;
    }
    
    if(        sum > trueTime)
    {
      sum = 0;
      state = true;
    }
    else if(-1*sum > falseTime)
    {
      sum = 0;
      state = false;
    }
    
    return state;
  }
  
 private:
  unsigned int trueTime, falseTime;
  double threshold;
  unsigned long lastTime;
  long sum;
  bool state;
};


// peram 0 - default to the clock on starting.
// peram 1 - time the signal needs to be stable on
// peram 2 - time the signal needs to be stable off
// peram 3 - threshold value
//TimeHysteresis(bool DefaultState, unsigned int trueTime, unsigned int FalseTime, double Threshold)
TimeHysteresis HysteState(false, 15000, 1000, 1200.0f);

unsigned long everySecond;


class AprroxTimer
{
 public:
  AprroxTimer(uint16_t Seconds) :
    offset(Seconds),
    lastTime(millis())
  {  }
 
  bool hit(const unsigned int &time)
  {
    return ( lastTime + offset <= time );
  }
 
  unsigned int diffms(const unsigned int &time) const
  {
    // This if case should never happen
    if(lastTime < time)  {
      return (time - lastTime);
    }
    return (lastTime - time);
  }
 
  void reset(const unsigned int &time)
  {
    lastTime = time;
  }
  
 private:
  unsigned int lastTime;
  const uint16_t offset;
};



AprroxTimer ReportFrameRate(10000); // every 10 seconds
uint16_t FrameCount = 0;

void loop()
{
  unsigned int timeStamp = millis();
  
  // Update the led on the teensy
  BlinkStatus.update(timeStamp);
  
  if (myFFT.available())
  {
    // Lock at the total signal comming into 
    double rms = RMS();
 
    if( HysteState.test(rms,timeStamp) )
    {
      updateEqualizer();
    }
    else
    {
      updateClock();
    }
    
    // Report the frame rate
    if(ReportFrameRate.hit(timeStamp))
    {
      unsigned int SecondCount = ReportFrameRate.diffms(timeStamp)/1000;
      Serial.print("  FFT Rate: ");
      if(SecondCount == 0)
      {
        Serial.print("0 ... weird this usally suggests extreamly fast/slow?");
      }
      else
      {
        Serial.print( (double)FrameCount/SecondCount);
      }
      Serial.println(" fps");
      
      SecondCount = 0;
      FrameCount = 0;
      ReportFrameRate.reset(timeStamp);
    }
    else
    {
      ++FrameCount;
    }
  }
}
        
//  ################## Main Loop Support Functions ###################

// ---- Function that calculates the RMS ERROR based on the FFT ----
double RMS()
{
  int index=0, temp;
  double val = 0;
  for(index=1; index<128; ++index)
  {
    temp = myFFT.output[index];
    val += temp*temp;
  }
  return sqrt((double)val);
}

void updateClock()
{
  if( CurrentClock >= Clocks.size() ) return;
  if( Clocks[CurrentClock] == NULL) return;
  
  Clocks[CurrentClock]->display(&matrix);
}

void updateEqualizer()
{  
  matrix.fillScreen(BLACK);
  
  uint16_t *FFT_Data = myFFT.output;
  for(uint8_t index=0; index<CurrentEqualizerSceen.size(); ++index)
  {
    CurrentEqualizerSceen[index]->display(FFT_Data, &matrix);
  }
  
  matrix.swapBuffers(true);
}

/*
const int BinMod = 128/MATRIX_WIDTH;

    //printStats();
    displayOne();
    //displayTwo();

int8_t octiveBiasIndex[128] = {1,2,3,3,4,4,4,4,5,5,5,5,5,5,5,5,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8};
//float  octiveBiasValue[8] = {0.125,0.25,0.5,1,2,4,8,16};
float  octiveBiasValue[8] = {0.0625, 0.125, 0.25, 0.5, 0.375, 0.75, 1.5, 3};

int8_t LogRemap[128] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,15,16,16,17,17,18,18,18,19,19,19,20,20,21,21,21,22,22,22,22,23,23,23,23,23,24,24,24,24,24,24,25,25,25,25,25,25,25,26,26,26,26,26,26,26,26,27,27,27,27,27,27,27,27,27,27,27,28,28,28,28,28,28,28,28,28,28,28,28,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,31,31,31,31,31,31,31,31,31,31};


int16_t flipMatrix(const int16_t &y)
{
  return matrix.getScreenHeight()-y-1;
}

void displayOne()
{
  //const float VertScale = 2.0f;
  const float VertScale = 0.005f;
  int X, Y, i, bi, XT;
  float b;
  
  //myFFT.output[0] =/4;
  //myFFT.output[1] =/2;
  
  X = 0;
  for (i=0; i<128; i++)
  {
    bi = octiveBiasIndex[i] -1;
    b = octiveBiasValue[bi];
    
    // Figure out vertical of current LED
    Y = (int)(VertScale*(myFFT.output[i]*b+1)); //log
    
    XT = LogRemap[i];
    
    matrix.drawPixel((int16_t)XT , flipMatrix(Y), Colors[(XT*8)%128]); //i
    
    
    
    if((i+1)%BinMod == 0)
    {
      // Increment to next column
      X++;
    }
  }
}
   
    */
/*
void displayTwo()
{
  const int Window = 128/32;
  float tempMatrix[MATRIX_WIDTH];
  int count[MATRIX_WIDTH];
  int X, Xp, Y, i, sum;
  
  for(i=0; i<MATRIX_WIDTH; ++i)
  {
    tempMatrix[i] = 0;
    count[i] = 0;
  }
  
  float range;
  float max, min, val;
  
  float verticalBase = log(2);
  float horizonalBase = log(2);
  
  max = log(128);
  min = log(1);
  range = (max-min);
  
  //myFFT.output[0] = 0;
  //myFFT.output[1] = 0;
  
  X = 0;
  sum = 0;
  for (i=0; i<128; i++)
  {
    //x'i = (log(xi)-log(xmin)) / (log(xmax)-log(xmin))
    //X =  (int)(MATRIX_WIDTH*((log(i+1)-min) / (range)));
    //X = i/BinMod;
    X = LogRemap[i];
    
    tempMatrix[X] += log(myFFT.output[i]+1);
    count[X]++;
    
    
    //sum += (int)(VertScale*myFFT.output[i]);
    
    //if((i+1)%BinMod == 0)
    //{
    //  tempMatrix[X] = log( (float)sum/BinMod + 1);
    //  
    //  X++;
    //  sum = 0;
    //}
    
  }
  
  for(i=0; i<matrix.getScreenWidth(); ++i)
  {
    if(count[i] != 0)
    {
      tempMatrix[i] /= (float)(count[i]);
    }
  }
  
  max = -32767;
  min = 32767;
  
  for(i=0; i<matrix.getScreenWidth(); ++i)
  {
    val = tempMatrix[i];
    sum += val;
    if(max < val)  {
      max = val;
    }
    if(min > val)  {
      min = val;
    }
  }
  
  float avg = (float)sum/matrix.getScreenWidth();
  range = max-min;
  Serial.print("FFT: [");
  Serial.print(min);
  Serial.print(", (");
  Serial.print(avg);
  Serial.print("), ");
  Serial.print(max);
  Serial.print("] ");
  
  int rangeLock = 15;
  if(range < rangeLock)
  {
    range = rangeLock;
  }
  
  Serial.print("r:");
  Serial.print(range);
  Serial.print(") ");
  
  
  range = (float)matrix.getScreenHeight()/(range);
  
  for(i=0; i<MATRIX_WIDTH; ++i)
  {
    val = tempMatrix[i];
    
    //if( val < 2.0) val = min;
    
    val = range*(val-min);
    
    tempMatrix[i] = (int)val ;//- 1; // remove bar
  }
  
  
  Serial.print(" => ");
  
  for(i=0; i<MATRIX_WIDTH; ++i)
  {
    Y = tempMatrix[i];
    
    Serial.print(Y);
    Serial.print(", ");
    
    matrix.drawPixel((int16_t)i, flipMatrix(Y), Colors[i*BinMod]);
    if( Y > 0 )
    {
      //matrix.drawFastVLine((int16_t)i, 0, (int16_t)Y, Colors[i*BinMod]);
    }
    
  }
}*/


