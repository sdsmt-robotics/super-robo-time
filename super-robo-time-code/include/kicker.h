#ifndef __SRT_KICKER
#define __SRT_KICKER

#include <Arduino.h>

class SRTKicker
{
private:   
  const int PWMFreq = 5000;     //5 KHz 
  const int PWMChannel = 13;     
  const int PWMResolution = 10;
  int rotation = (int)(pow(2, PWMResolution) - 1);

public:
  SRTKicker(int _pin);
  void kickerOn();
  void kickerOff();   //add function definitions below, and implement the functions in kicker.cpp
};


#endif
