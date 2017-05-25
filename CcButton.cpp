/**
 * CcButton.cpp - Library MIDI CC Controls.
 * Creates a button that toggles between two values of a specific
 * MIDI control along with corresponding LED.
 */

#include "Arduino.h"
#include "Bounce.h"
#include "MIDI.h"
#include "CcButton.h"

CcButton::CcButton(
  int btnPin,
  int ledPin,
  int state,
  int previous,
  int valueOff,
  int valueOn,
  int cc,
  int debounceMS,
  char ccName[]
) : _btn(btnPin, debounceMS) {
  _btnPin = btnPin;
  _ledPin = ledPin;
  _state = state;
  _previous = previous;
  _valueOff = valueOff;
  _valueOn = valueOn;
  _cc = cc;
  _ccName = ccName;
}


/**
 * Initializes inputs and outputs.
 */
void CcButton::begin() {
  pinMode(_btnPin, INPUT_PULLUP);
  pinMode(_ledPin, OUTPUT);
  digitalWrite(_ledPin, _previous);
}


/**
 * Computes changes on  inputs and outputs.
 * Sends MIDI messages and toggles LEDs based on current state.
 * @param {int} midiChannel MIDI channel to send message on.
 */
void CcButton::update(int midiChannel) {
  _btn.update();
  _reading = _btn.read();

  if (_reading != _previous && _btn.fallingEdge()) {
    if (_state == HIGH) {
      usbMIDI.sendControlChange(_cc, _valueOn, midiChannel);
      MIDI.sendControlChange(_cc, _valueOn, midiChannel);
      // sprintf(buffer, "Toggle %s led on: %x", _ccName, _valueOn);
    } else {
      usbMIDI.sendControlChange(_cc, _valueOff, midiChannel);
      MIDI.sendControlChange(_cc, _valueOff, midiChannel);
      // sprintf(buffer, "Toggle %s led off: %x", _ccName, _valueOff);
    }
    // Serial.println(buffer);
    digitalWrite(_ledPin, _state);

    _state = !_state;
  }
  _previous = _reading;
}
