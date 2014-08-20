/*
  Control.h - Library MIDI CC Controls.
*/
#ifndef Control_h
#define Control_h

#include "Arduino.h"

class Control
{
  public:
    Control(int pin);
  private:
    int _pin;
};

#endif
