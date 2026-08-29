#include "kicker.h"
#include "Arduino.h"

SRTKicker::SRTKicker(int _pin):_pin(_pin)
{
}

void SRTKicker::kickerOn()
{
  pwm.write(_pin, 255, PWMFreq, PWMResolution);
}

void SRTKicker::kickerOff()
{
  pwm.write(_pin, 0, PWMFreq, PWMResolution);
}

void SRTKicker::init()
{
  pinMode(_pin, OUTPUT);
  pwm.attach(_pin);
}
