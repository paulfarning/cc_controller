/*
  CcButton.cpp - Library for MIDI CC buttons.
*/

#include "Arduino.h"
#include "Bounce.h"
#include "CcButton.h"

CcButton::CcButton(int btnPin, int ledPin, int state, int previous, int stateOn, int stateOff, int cc, char ccName[]) : _btn(btnPin, 100) {
  _btnPin = btnPin;
  _ledPin = ledPin;
  _state = state;
  _previous = previous;
  _stateOn = stateOn;
  _stateOff = stateOff;
  _cc = cc;
  _ccName = ccName;
}


void CcButton::begin() {
  pinMode(btnPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, previous);
}


void CcButton::update() {

  _btn.update();
  _reading = _btn.read();

  if (_reading != _previous && _btn.fallingEdge()) {
    if (_state == HIGH) {
      usbMIDI.sendControlChange(_cc, _stateOff, midiChannel);
      MIDI.sendControlChange(_cc, _stateOff, midiChannel);
      sprintf(buffer, "Toggle %s led on: %x", _ccName, _stateOff);
    } else {
      usbMIDI.sendControlChange(_cc, _stateOn, midiChannel);
      MIDI.sendControlChange(_cc, _stateOn, midiChannel);
      sprintf(buffer, "Toggle %s led off: %x", _ccName, _stateOn);
    }
    Serial.println(buffer);
    digitalWrite(_ledPin, _state);

    _state = !_state;
  }
  _previous = _reading;

}
