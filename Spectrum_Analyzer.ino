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

#include "DecayClasses.h"
#include "DisplayLimitDecay.h"   //Shows the max Y in a column and decays it in some way.

#include "DisplayPersistant.h" //Shows a history of pixels that are decaying in some way.

#include "SpectrumDisplay.h" // Class to mannage a equalizer screen, or setup.

#include "BlinkPatternManager.h" // Just blinks the status led
#include "TimeHysteresis.h"      // Class to solve the noisy input signal
#include "AprroxTimer.h"         // Just a simple timer that is used to spit out diag. info

// System includes
#include "ArraySharedPtr.h"


#include <memory>
using std::shared_ptr;
// ######################## System Defines ########################
// 3 dec doubling in energy 1 is lowest
// every octive is a doubling in frequency

//---- Display ----
SmartMatrix matrix;
const int INT_SCREEN_DELAY = 500/3;
const int DefaultBrightness = 25*(255/100);
const rgb24 BLACK = {0,0,0};


//---- Audio  ----
/*
  AudioWindowHanning256
  AudioWindowBartlett256
  AudioWindowBlackman256
  AudioWindowFlattop256
  AudioWindowBlackmanHarris256
  AudioWindowNuttall256
  AudioWindowBlackmanNuttall256
  AudioWindowWelch256
  AudioWindowHamming256
  AudioWindowCosine256
  AudioWindowTukey256
*/
AudioAnalyzeFFT256  myFFT(4,0);//(10); 4
AudioInputAnalog   audioInput(17,DEFAULT);  // Pin 17 = A3, Default = 0-3.3v

AudioConnection Connection(audioInput, myFFT);

//---- Time & Display ----
uint8_t CurrentClock = 1;
ArraySharedPtr<ClockBase> Clocks;
//TODO change this into a list of function pointers that return a shared_ptr
// so that only the current clocks display is instatiated

//---- Equalizer Display
uint8_t CurrentEqualizer = 0;
ArraySharedPtr<SpectrumDisplay> CurrentEqualizerScreen;
//TODO change this into a list of function pointers that return a shared_ptr
// so that only the current specrum display is instatiated

// ---- Status Led ----;
BlinkPatternManager blinker({1000,1000,100,500},13);
// Pattern on pin 13 - off(1s), on(1s), off(0.1), on(0,5)

std::shared_ptr<SpectrumDisplay> generateMaxDisplay(const MatrixSize &matrixSize)
{
  std::shared_ptr<SpectrumDisplay> specDisp = std::make_shared<SpectrumDisplay>();
  ArraySharedPtr<  RemapBase  > tempRemappers;
  ArraySharedPtr< DisplayBase > tempDisplayers;
  
  
  // Compression - Avg, All, Max
  
  Serial.println("    Data Remappers ...");
  // ---- Set up equalizer (this is the basic verision ----
  tempRemappers.initalize( 2 );
  Serial.println("      Octave ...");
  tempRemappers[0] = std::make_shared<RemapOctave>( matrix.getScreenWidth(), Compression::Max );
  Serial.println("      - done");
  Serial.println("      Decimal ...");
  shared_ptr<RemapDecibel> TempDecimalptr = std::make_shared<RemapDecibel>(matrixSize);
  TempDecimalptr->lockVert(0.2f);
  tempRemappers[1] = TempDecimalptr;
  Serial.println("      - done");
  //Serial.println("      Linear ...");
  //tempRemappers[2] = std::make_shared< RemapLinear >( RemapLinear::ScaleY,
  //                                                    matrixSize,
  //                                                    Compression::None          );
  //Serial.println("      - done");
  Serial.println();
  
  Serial.println("    Data renders ...");
  tempDisplayers.initalize(2);
  Serial.println("      Basic render ...");
  
  tempDisplayers[0] = std::make_shared<DisplayRaw>();
  
  shared_ptr<DisplayLimitDecay> tempDisplayMaxDecayPtr;
 
  tempDisplayMaxDecayPtr = std::make_shared<DisplayLimitDecay>(DisplayLimitDecay::Max);
  
  ArraySharedPtr<DecayFunciton> decayFunction;
  decayFunction.initalize(2);
  
  // double Gravity, double TimeScale, uint16_t hold):
  const double GravitySec = 0.1; //= d/s^2
  const double holdSec = 0.5;
  const uint8_t decayFade = 2;
  const double TimeScale = 1.0/60.0;  // the 1/60 const comes from an aproximant framerate

  const uint16_t hold = (uint16_t)(holdSec/TimeScale);
  const double gravity = GravitySec*TimeScale*TimeScale;
  
  
  decayFunction[0] = std::make_shared<DecayGravity>(gravity, TimeScale, hold);
  decayFunction[1] = std::make_shared<DecayFade>(decayFade);
  tempDisplayMaxDecayPtr->setDecayFunction( decayFunction );
  
  tempDisplayers[1] = tempDisplayMaxDecayPtr;
  
  
  Serial.println("      Registering Setup");  
  specDisp->registerRemappers(tempRemappers);
  specDisp->registerDisplayers(tempDisplayers);
  tempRemappers.clear();
  Serial.println("    - done");
  Serial.println();
  
  return specDisp;
}

std::shared_ptr<SpectrumDisplay> generatePersistantDisplay(const MatrixSize &matrixSize)
{
  std::shared_ptr<SpectrumDisplay> specDisp = std::make_shared<SpectrumDisplay>();
  ArraySharedPtr<  RemapBase  > tempRemappers;
  ArraySharedPtr< DisplayBase > tempDisplayers;
  ArraySharedPtr<DecayFunciton> decayFunction;
  
  shared_ptr<RemapDecibel> tempDecimalptr;
  shared_ptr<DisplayPersistant> tempDisplayDecayPtr;
  
  // double Gravity, double TimeScale, uint16_t hold):
  const double GravitySec = 1; //= d/s^2
  const double holdSec = 0;
  const uint8_t decayFade = 5;
  const double TimeScale = 1.0/60.0;  // the 1/60 const comes from an aproximant framerate

  const uint16_t hold = (uint16_t)(holdSec/TimeScale);
  const double gravity = GravitySec*TimeScale*TimeScale;
  
  
  // Initalize data remappers
  tempRemappers.initalize( 2 );
  tempRemappers[0] = std::make_shared<RemapOctave>( matrix.getScreenWidth(), Compression::Avg );
  
  tempDecimalptr = std::make_shared<RemapDecibel>(matrixSize);
  tempDecimalptr->lockVert(0.15f);
  tempRemappers[1] = tempDecimalptr;
  
  // Initalize how the draw function moves the buffered data;
  decayFunction.initalize(1);
  decayFunction[0] = std::make_shared<DecayGravity>(gravity, TimeScale, hold);
  //decayFunction[1] = std::make_shared<DecayFade>(decayFade);
  
  // Intalize renders
  tempDisplayers.initalize(1);
  tempDisplayDecayPtr = std::make_shared<DisplayPersistant>();
  tempDisplayDecayPtr->setDecayFunction( decayFunction );
  tempDisplayers[0] = tempDisplayDecayPtr;
  
  
  specDisp->registerRemappers(tempRemappers);
  specDisp->registerDisplayers(tempDisplayers);
  return specDisp;
}

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
  
  MatrixSize matrixSize(matrix);
  Serial.println("Screen Initalized");
  Serial.print("Screen is ");
  Serial.print(matrixSize.width);
  Serial.print(" wide by ");
  Serial.print(matrixSize.height);
  Serial.println(" tall");  


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
  // ---- Set up scence builder ----
  CurrentEqualizerScreen.initalize(1);
  //CurrentEqualizerScreen[0] = generatePersistantDisplay(matrixSize);
  CurrentEqualizerScreen[0] = generateMaxDisplay(matrixSize);
  Serial.println("  - done");
  
  
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
TimeHysteresis HysteState(false, 150, 100, 244.0f, TimeStateChangeNotifier);

AprroxTimer ReportFrameRate(10000); // every 10 seconds
uint16_t FrameCount = 0;
long startTime = 0, endTime = 0;
double RMS_S = 0;

void loop()
{
  unsigned int timeStamp = millis();
  
  // Update the led on the teensy
  blinker.update(timeStamp);
  
  if (myFFT.available())
  {
    // Lock at the total signal comming into 
    double rms = RMS();
    RMS_S += rms;
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
      Serial.print((endTime - startTime)/((double)SecondCount*1000)*100);
      
      Serial.print("% ");
      
      Serial.print("Avg RMS: ");
      Serial.print( RMS_S/((double)FrameCount) );
      
      Serial.println();
      
      SecondCount = 0;
      FrameCount = 0;
      startTime = 0;
      endTime = 0;
      RMS_S = 0;
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
  
  // Copy the data from teh FTT buffer to the display buffer
  uint8_t index;
  for(index=0; index<128; ++index)
  {
    EqualizerBuffer.data[index].X = index;
    EqualizerBuffer.data[index].Y = myFFT.output[index];
    EqualizerBuffer.data[index].C = HSV_Colors[index];
  }
  EqualizerBuffer.size = 128;

  // Call the current equalizer  
  CurrentEqualizerScreen[CurrentEqualizer]->display(EqualizerBuffer,&matrix);
}




