#include "kicker.h"
#include "Arduino.h"

SRTKicker::SRTKicker(int _pin):_pin(_pin)
{
}

void SRTKicker::kickerOn()
{
  analogWrite(_pin, 255);
}

void SRTKicker::kickerOff()
{
  analogWrite(_pin, 0);
}

void SRTKicker::init()
{
  pinMode(_pin, OUTPUT);
}