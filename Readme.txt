ReadMe File for Spectrum Analyzer, this project was built upon a teensy 3.1 and a 16x32 or a 32x32 RGB display. More information can be found at http://pure8spark.com/?page_id=100.

For now, Copyrighted 2014 Brian Hamilton All rights reserved. (Tell I decide on a license that is least limiting based on the libraries used). 


This project is split up into features.

Spectrum analyzer
Clock, why not
All supporting code

Code is much simpler then it looks, polymorphism might have been overly used in this project.

Spectrum_Analyzer (main)
 = RemapBase (base class) (all remappers ignore FFT_output.data[0] because this is the dc component of the signal)
 -   RemapDecibel remaps amplitude data by the log approximately.
 -   RemapOctave remaps the x axis into a frequency doubling approximately. 
 -   RemapLinear scales the data.

 = DisplayBase (base class) inherited by all classes that display to the screen.
 
 = ClockBase (base class)
 -   ClockBasic just displays the hour and minutes
 -   ClockFull Shows day, time, and date.


Motivation - make code extremely simple to add on, would be nice for community to contribute back.


TODO:
 - Add Night sleep mode
 - Fix clock - is being initialized to the correct, reports the correct time but displays 2:XX ?
 - Move Teensy clock code back into its own library again.
 - keywords file?
 - Fix FFT, window?
 - Build prototype, and stop using hack job
 - !! Menu items for remote interface
 - Have way for classes to report their parameters to the menu system.
 - Finish rewrite of adc.h to read stereo instead of using mix circuit. 


Done
 - Finish code rewrite.