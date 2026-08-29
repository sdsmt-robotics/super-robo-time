#ifndef __SRT_KICKER
#define __SRT_KICKER

#include <Arduino.h>
#include <pwmWrite.h> // https://github.com/Dlloydev/ESP32-ESP32S2-AnalogWrite

class SRTKicker
{
private:
  int _pin;                    
  Pwm pwm = Pwm();             
  const int PWMFreq = 5000;    
  const int PWMResolution = 8; 

public:
  SRTKicker(int _pin);
  void init();
  void kickerOn();
  void kickerOff();   //add function definitions below, and implement the functions in kicker.cpp
};


#endif
