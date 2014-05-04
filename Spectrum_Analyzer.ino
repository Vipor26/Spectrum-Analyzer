// This project was writen by Brian Hamilton

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
#include "ClockBase.h"  // Base Class
#include "ClockBasic.h" // Shows HH:MM
#include "ClockFull.h"  // Shows the day, time, and date, but in a basic white small text.

// The following are the includes for the fft remapp base class, these classes
//    map the input data onto the display for the display classes
#include "RemapBase.h"    // Base Class
#include "RemapLinear.h"  // Remap the X or Y axis
#include "RemapDecibel.h" // Remap the Y axis to logorithmic scale
#include "RemapOctave.h"  // Remap the X axis to an approximate doubling of frequency each bin.

// The following are the includes for the fft display base class, and all the
//    diffrent affects that can be placed on top of eachother.
#include "DisplayBase.h" // Base Class
#include "DisplayRaw.h"  // Just display what is viewable in the array nothing fancy.
#include "DisplayMaxDecay.h"   //Shows the max Y in a column and decays it in some way.
#define PERSISTANT_BUFFER_SIZE 4 //Defines the number of pixels to save per column in the DispayPersistant class.
#include "DisplayPersistant.h" //Shows a history of pixels that are decaying in some way.

#include "SpectrumDisplay.h" // Class to mannage a equalizer screen, or setup.

#include "BlinkPatternManager.h" // Just blinks the status led
#include "TimeHysteresis.h"      // Class to solve the noisy input signal
#include "AprroxTimer.h"         // Just a simple timer that is used to spit out diag. info

// System includes
#include "ArraySharedPtr.h"

// ######################## System Defines ########################
// 3 dec doubling in energy 1 is lowest
// every octive is a doubling in frequency

//---- Display ----
SmartMatrix matrix;
const int INT_SCREEN_DELAY = 1000;
const int DefaultBrightness = 25*(255/100);
const rgb24 BLACK = {0,0,0};


//---- Audio  ----
AudioAnalyzeFFT256  myFFT(4,0);//(10); 5
AudioInputAnalog   audioInput(17,DEFAULT);  // Pin 17 = A3, Default = 0-3.3v

AudioConnection Connection(audioInput, myFFT);

//---- Time & Display ----
uint8_t CurrentClock = 1;
ArraySharedPtr<ClockBase> Clocks;

//---- Equalizer Display
uint8_t CurrentEqualizer = 0;
ArraySharedPtr<SpectrumDisplay> CurrentEqualizerScreen;

// ---- Status Led ----;
BlinkPatternManager blinker({1000,1000,100,500},13);
// Pattern on pin 13 - off(1s), on(1s), off(0.1), on(0,5)

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

  matrix.fillScreen( RED );       //
  matrix.swapBuffers();         // swap buffers
  delay(INT_SCREEN_DELAY);                  // waits for a second

  matrix.fillScreen( GREEN );
  matrix.swapBuffers();
  delay(INT_SCREEN_DELAY);                  // waits for a second

  matrix.fillScreen( BLUE );
  matrix.swapBuffers();
  delay(INT_SCREEN_DELAY);                  // waits for a second

  matrix.fillScreen( BLACK );
  matrix.swapBuffers();

  
  // ---- Set up the serial debug ----
  // CTRL SHIFT M
  Serial.begin(115200);
  delay(1000);
  Serial.println("Screen Initalized");


  Serial.println();
  Serial.println("Initalizing ...");
  
  Serial.println("  Time ...");
  // ---- Set the clock ----
  //  Based on when this was compiled.
  SetFromCompilerTime(5 + 3*INT_SCREEN_DELAY); //takes 5 seconds to compile +3 to run to this point
  //  Print what the current time is
  Serial.print("  - ");
  SerialPrintTime();
  Serial.println();
  Serial.println("  - done");
  

  Serial.println("  Clocks ...");
  // ---- Set up clocks
  Clocks.initalize( 2 );
  Clocks[0] = std::make_shared<ClockBasic>();
  Clocks[1] = std::make_shared<ClockFull>();
  Serial.println("  - done");
  Serial.println();
  
  Serial.println("  Working on equalizer 1 ...");
  Serial.println("    Helper ...");
  // ---- Set up scence builder ----
  CurrentEqualizerScreen.initalize(1);
  CurrentEqualizerScreen[0] = std::make_shared<SpectrumDisplay>();
  Serial.println("    - done");
  
  ArraySharedPtr<  RemapBase  > tempRemappers;
  ArraySharedPtr< DisplayBase > tempDisplayers;
  
  
  Serial.println("    Data Remappers ...");
  // ---- Set up equalizer (this is the basic verision ----
  tempRemappers.initalize( 3 );
  Serial.println("      Octave ...");
  tempRemappers[0] = std::make_shared<RemapOctave>( matrix.getScreenWidth(), Compression::All );
  Serial.println("      - done");
  Serial.println("      Decimal ...");
  tempRemappers[1] = std::make_shared<RemapDecibel>();
  Serial.println("      - done");
  Serial.println("      Linear ...");
  tempRemappers[2] = std::make_shared< RemapLinear >( RemapLinear::ScaleXY,
                                                      matrix.getScreenHeight(),
                                                      matrix.getScreenWidth(),
                                                       Compression::All          );
  Serial.println("      - done");
  Serial.println("      Uploading and clearing temp");  
  //CurrentEqualizerScreen[0]->registerRemappers(tempRemappers);
  //tempRemappers.clear();
  Serial.println("    - done");
  Serial.println();
  
  Serial.println("    Data renders ...");
  tempDisplayers.initalize(1);
  Serial.println("      Basic render ...");
  tempDisplayers[0] = std::make_shared<DisplayRaw>();
  Serial.println("      - done");
  Serial.println("      Uploading and clearing temp");  
  //CurrentEqualizerScreen[0]->registerDisplayers(tempDisplayers);
  Serial.println("    - done");
  Serial.println();
  
  Serial.println("Initalizing Complete");
}

//  ########################## Main Loop ##########################

//  ---- Function prototypes ----
void updateLED();
void updateClock();
void updateEqualizer();
double RMS();
void TimeStateChangeNotifier(bool);

// ---- Logic global declerations ----

// peram 0 - default to the clock on starting.
// peram 1 - time the signal needs to be stable on
// peram 2 - time the signal needs to be stable off
// peram 3 - threshold value
//TimeHysteresis(bool DefaultState, unsigned int trueTime, unsigned int FalseTime, double Threshold)
TimeHysteresis HysteState(false, 15000, 1000, 1200.0f, TimeStateChangeNotifier);

AprroxTimer ReportFrameRate(10000); // every 10 seconds
uint16_t FrameCount = 0;
long startTime = 0, endTime = 0;
double RMS_S = 0;
unsigned long LoopCount = 0;

void loop()
{
  unsigned int timeStamp = millis(); 
  ++LoopCount;
  
  // Update the led on the teensy
  blinker.update(timeStamp);
  
  if (myFFT.available())
  {
    // Lock at the total signal comming into 
    double rms = RMS();
    RMS_S += rms;
    if( HysteState.test(rms,timeStamp) )
    {
      //updateEqualizer();
    }
    else
    {
      updateClock();
    }
    
    // Report the frame rate
    if(ReportFrameRate.hit(timeStamp))
    {
      unsigned int SecondCount = ReportFrameRate.diffms(timeStamp)/1000;
      Serial.print("LoopRate: ");
      if(SecondCount == 0)
      {
        Serial.print("0 ... weird this usally suggests extreamly fast/slow?");
      }
      else
      {
        Serial.print( (double)FrameCount/SecondCount);
      }
      Serial.print(" fps ");
      
      Serial.print("  Proc On: ");
      Serial.print((endTime - startTime)/((double)SecondCount*1000));
      
      //Serial.print("%, off: ");
      //Serial.print((startTime - endTime)/((double)SecondCount*1000));
      
      Serial.print("% ");
      
      Serial.print("Avg RMS: ");
      Serial.print( RMS_S/((double)FrameCount) );
      
      Serial.print(" hit: ");
      Serial.print( FrameCount/((double)LoopCount) );
      Serial.print(" %");
      
      Serial.println();
      
      SecondCount = 0;
      FrameCount = 0;
      startTime = 0;
      endTime = 0;
      RMS_S = 0;
      LoopCount = 0;
      ReportFrameRate.reset(timeStamp);
    }
    else
    {
      ++FrameCount;
    }
    startTime += timeStamp;
    endTime   += millis();
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

void TimeStateChangeNotifier(bool state)
{
  if(state)
  {
    Serial.println(" -- Equalizer mode --");
  }
  else
  {
    Serial.println(" -- Clock mode --");
  }
}

void updateClock()
{
  if( CurrentClock >= Clocks.size() ) return;
  if( Clocks[CurrentClock] == NULL) return;
  
  Clocks[CurrentClock]->display(&matrix);
}

DataBuffer EqualizerBuffer;

void updateEqualizer()
{
  
  if( CurrentEqualizer >= CurrentEqualizerScreen.size() ) return;
  if( CurrentEqualizerScreen[CurrentEqualizer] == NULL) return;
  
  uint8_t index;
  for(index=0; index<128; ++index)
  {
    EqualizerBuffer.data[index].X = index;
    EqualizerBuffer.data[index].Y = myFFT.output[index];
  }
  EqualizerBuffer.size = 128;
  
  CurrentEqualizerScreen[CurrentEqualizer]->display(EqualizerBuffer,&matrix);
}

/*
This was the original experiment code to be removed once the completed system is up and running
const int BinMod = 128/MATRIX_WIDTH;

    //printStats();
    displayOne();
    //displayTwo();

int8_t octiveBiasIndex[128] = {1,2,3,3,4,4,4,4,5,5,5,5,5,5,5,5,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8};
//float  octiveBiasValue[8] = {0.125,0.25,0.5,1,2,4,8,16};
float  octiveBiasValue[8] = {0.0625, 0.125, 0.25, 0.5, 0.375, 0.75, 1.5, 3};




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


