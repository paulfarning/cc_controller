/*
  CcButton.h - Library MIDI CC Controls.
*/
#ifndef CcButton_h
#define CcButton_h

#include "Arduino.h"
// class Bounce;

class CcButton {
  public:
    CcButton(int btnPin, int ledPin, int state, int previous, int stateOn, int stateOff, int cc, char ccName[]);
    void begin();
    void update();
  private:
    int _btnPin;
    int _ledPin;
    int _state;
    int _previous;
    int _stateOn;
    int _stateOff;
    int _cc;
    char * _ccName;
    Bounce _btn;
    int _reading;
};

#endif
