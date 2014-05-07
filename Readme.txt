ReadMe File for Spectrum Analyzer, this project was built upon a teensy 3.1 and a 16x32 or a 32x32 RGB display (maybe more configurations will be supported).
These displays are color !!!


This code relies on both the Teensy Audio and SmartMatrix Libararies.
 - Audio https://github.com/PaulStoffregen/Audio
   - Note: that the DMA channels must be moved to used the A2dc with the
           display code and I will post that code soon.
 - Smart Matrix https://github.com/pixelmatix/SmartMatrix

More information can be found at http://pure8spark.com/?page_id=100.

For now, Copyrighted 2014 Brian Hamilton All rights reserved. (Tell I decide on
a license that is least limiting based on the libraries used).


=============================================================================
= This code is under heavy development and is not yet in a releasable state =
=   this repo is for recommendation and for the curious observer but will   =
=   be in a more complete state soon.                                       =  
=============================================================================

The goal of this project is to make an amazing audio spectrum analyzer which
requires the minimal code rewrite when adding a new way for the data to be
visualized. This will be accomplished using a design pattern called composite.
Or C++ polymorphism depending on your schooling.


Currently the project is aiming to have
——————————————————————————————————————
 - Clock when no music is being played
 - Multiple spectrum displays when the music is on
 - and a remote.


Code is much simpler then it looks, polymorphism might have been overly used in
this project. The idea is to abstract building blocks and support code so that
the end user does not need to significantly change code to add a different
display.
——————————————————————————————————————
 = Spectrum_Analyzer (main driver)
 
 = RemapBase (base class) (all remappers ignore FFT_output.data[0] because
     this is the dc component of the signal) 

 -   RemapDecibel remaps amplitude data by the log approximately.
 -   RemapOctave  remaps the x axis into a frequency doubling approximately.
 -   RemapLinear scales the data.

 = DisplayBase (base class) inherited by all classes that display to the screen.
 -   DisplayRaw just pushes the data buffer to the screen
 -   DisplayMax displays the current jumping data with a max that is decaying in
       some way defined be the decay function.
 -   DisplayPersistant displays the current value and a recent history of values
       which are decaying in some way.

 = ClockBase (base class)
 -   ClockBasic just displays the hour and minutes
 -   ClockFull Shows day, time, and date.


Persistant display was motivated by http://en.wikipedia.org/wiki/File:Comparison_of_Max_Hold_Spectrum_Analyzer_trace_and_Persistence_Trace.png
Max display is the more traditional way.

  TODO
——————————————————————————————————————
 - Find bugs in remappers
 - DisplayMaxDecay
 - Write Combiner to take an axis argument 
 - Write Y Axis Combiner into Rempap Liner.y and RemapDecible?
 - DisplayPersistant - State information in data?
 - Add sleep mode, Night and no sound?
 - Fix clock
 - is being initialized to the correct, reports the correct time but displays 2:XX ?
 - Move Teensy clock code back into its own library
again.
 - keywords file?
 - Fix FFT, window?
 - Build prototype, and stop using hack job
 - !! Menu items for remote interface
 - Have way for classes to report their parameters to the menu system.
 - Finish rewrite of adc.h to read stereo instead of using mix circuit.
 - store settings to eeprom -   #include <EEPROM.h>
 -   100,000 write/erase cycles -   EEPROM.write(address, value)
 -     address: the location to write to, starting from 0 (int)
 -     value: the value to write, from 0 to 255 (byte)
 -   byte EEPROM.read(address)
 - https://learn.adafruit.com/memories-of-an-arduino/measuring-free-memory


  Done
——————————————————————————————————————
 - Finish code rewrite.
 - Have two clocks written
 - Have three remap functions
 - Have three displays