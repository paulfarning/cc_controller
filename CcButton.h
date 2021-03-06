/**
 * CcButton.h - Library MIDI CC Controls.
 * Creates a button that toggles between two values of a specific
 * MIDI control along with corresponding LED.
 */

#ifndef CcButton_h
#define CcButton_h

#include "Arduino.h"

class CcButton {
  public:
    CcButton(
      int btnPin,       // Pin button is on.
      int ledPin,       // Pin LED is on.
      int state,        // Current state of LED.
      int previous,     // Initial/previous state of LED.
      int valueOff,     // Initial CC value (LED off).
      int valueOn,      // Toggled CC value (LED on).
      int cc,           // CC number.
      int debounceMS,   // Debounce time in milliseconds.
      char ccName[]     // Name for control.
    );
    void begin();       // Initialize. Call from setup.
    void update(int midiChannel); // Process. Call from loop.
  private:
    int _btnPin;        // Pin button is on.
    int _ledPin;        // Pin LED is on.
    int _state;         // Current state of LED.
    int _previous;      // Initial/previous state of LED.
    int _valueOff;      // Initial/off CC value.
    int _valueOn;       // On CC value.
    int _cc;            // CC number.
    char * _ccName;     // Name for control.
    int _reading;       // Current state of button.
    Bounce _btn;        // Debounced button.
};

#endif
