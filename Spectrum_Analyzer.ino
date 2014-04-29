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

// ######################## Includes ########################
// Include audio
#include <input_adc.h>
#include <analyze_fft256.h>
#include <memory>
//#include <vector>

// Include Screen
#include <SmartMatrix.h>

// Include definsions for 128 colors in HSV H = 360/128*i
#include "HSV_Color.h" 

#include "ClockBase.h"
#include "BasicClock.h"

// ######################## System Defines ########################
// 3 dec doubling in energy 1 is lowest
// 

//---- Display ----
SmartMatrix matrix;
const int INT_SCREEN_DELAY = 1000;
const int DefaultBrightness = 25*(255/100);
rgb24 BLACK = {0,0,0};


//---- Audio  ----
AudioAnalyzeFFT256  myFFT(4,0);//(10); 5
AudioInputAnalog   audioInput(17,DEFAULT);  // Pin 17 = A3, Default = 0-3.3v

AudioConnection Connection(audioInput, myFFT);

//---- Time ----
std::shared_ptr<ClockBase> CurrentClock = std::make_shared<BasicClock>();
//std::shared_ptr<ClockBase> CurrentEqualizer = std::make_shared<BasicEqualizer>();


//---- Status Led ----
const int ledPin = 13;              // LED connected to digital pin 13
bool ledState = false;
unsigned long ledPreviusStateChange = 0;
const unsigned long ledOnInterval = 100;           // (milliseconds)
const unsigned long ledOffInterval = 1000;         // (milliseconds)



//  ######################## System set up ########################
void setup() {

  SetFromCompilerTime(5);
  
  // ---- Set up audio ----
  AudioMemory(12);
  

  // ---- Set up the led ----
  pinMode(ledPin, OUTPUT);      // sets the digital pin as output
  digitalWrite(ledPin, true);
  ledState = true;

  // ---- Set up the screen with a little test sequence ----
  rgb24 RED   = {255,0,0};
  rgb24 GREEN = {0,255,0};
  rgb24 BLUE  = {0,0,255};
  
  matrix.begin();
  matrix.setBrightness(DefaultBrightness);
  //matrix.setScrollOffsetFromEdge(defaultScrollOffset);
  matrix.setColorCorrection(cc24);
  
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
  
  // CTRL SHIFT M
  Serial.begin(9600);
  Serial.println("System Initalized");
  //audioInput.begin(A4,DEFAULT);
}

//  ########################## Main Loop ##########################

void updateLED();
void updateDisplay();


bool signalOutLock = true;
double lock = 1200;
unsigned long onHyst, offHyst;

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



unsigned long everySecond;


void loop()
{
  updateLED();
  
  if (myFFT.available())
  {
    double rms = RMS();
    
    
    
    // if there is signal && currently set to off
    if(signalOutLock &&  (rms > lock))
    {
      // if this is the first time, since last on state change
      if(offHyst == 0)  {
        Serial.println("Maybe a Signal?"); 
        offHyst = millis();
        onHyst = 0;
      }
      else if(offHyst + 250 < millis()) // set hyreses for off
      {
        onHyst = 0;
        signalOutLock = false;
        Serial.println("Turning on"); 
      }
    }
    if(!signalOutLock && (rms < lock))
    {
      if(onHyst == 0)  {
        Serial.println("Maybe off?"); 
        onHyst = millis();
        offHyst = 0;
      }
      else if(onHyst + 15000 < millis())
      {
        offHyst = 0;
        signalOutLock = true;
        Serial.println("Turning off");
      }
    }
    
    if(signalOutLock)
    {
      if(everySecond + 1000 < millis())
      {
        everySecond = millis();
        
        matrix.fillScreen(BLACK);
        
        CurrentClock->display(&matrix);
        matrix.swapBuffers(true);
      }
    }
    else
    {
      updateDisplay();
    }
  } 
}



void updateLED()
{
  unsigned long currentTimeMs = millis();
  unsigned long ledInterval;
  
  if(ledState)  {
    ledInterval = ledOffInterval;
  }
  else  {
    ledInterval = ledOnInterval;
  }
  
  if( currentTimeMs - ledPreviusStateChange > ledInterval )
  {
    ledPreviusStateChange = currentTimeMs;
  
    digitalWrite(ledPin, ledState);
    ledState = !ledState;
  }
}


const int BinMod = 128/MATRIX_WIDTH;

void displayOne();
void displayTwo();
void printStats();





void updateDisplay()
{
  
    matrix.fillScreen(BLACK);
    
    //printStats();
    displayOne();
    //displayTwo();

    
    matrix.swapBuffers(true);
}

void printStats()
{
  int dc = audioInput.getDC();
  int low = audioInput.getLow();
  int high = audioInput.getHigh();
  Serial.print("DC: [(");
  Serial.print(low);
  Serial.print(")=");
  Serial.print( (low)/(65535.0)*3.3 );
  Serial.print(", (");
  Serial.print(dc);
  Serial.print(")=");
  Serial.print( (dc)/(65535.0)*3.3 );
  Serial.print(", (");
  Serial.print(high);
  Serial.print(")=");
  Serial.print( (high)/(65535.0)*3.3 );
  Serial.print("] ");
}


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
    
    /*
    sum += (int)(VertScale*myFFT.output[i]);
    
    if((i+1)%BinMod == 0)
    {
      tempMatrix[X] = log( (float)sum/BinMod + 1);
      
      X++;
      sum = 0;
    }
    */
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
}
