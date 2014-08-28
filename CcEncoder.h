/*
  CcEncoder.h - Library MIDI CC Controls.
  Creates an encoder that sets a value between 0 and 127.
*/

#ifndef CcEncoder_h
#define CcEncoder_h

#include "Arduino.h"

class CcEncoder {
  public:
    CcEncoder(
      int pin1,           // Leg 1 pin.
      int pin2,           // Leg 3 pin.
      int maxValue,       // Max value for encoder.
      int minValue,       // Min value for encoder.
      char ccName[]       // Name for control.
    );
    void begin();         // Initialize. Call from setup.
    void update();        // Process. Call from loop.
    int read();           // Get the current value of encoder.
    int showValue();      // Whether or not to display value.
    int getStartTime();   // Return time last changed.
  private:
    int _pin1;            // Leg 1 pin.
    int _pin2;            // Leg 3 pin.
    int _maxValue;        // Max value for encoder.
    int _minValue;        // Min value for encoder.
    char * _ccName;       // Name for control.
    QuadEncoder _encoder; // Encoder.
    int _value;           // Encoder value.
    int _displayValue = false;
    long _displayTimeout = 4000;
    long _startTime = 0;
};

#endif
