/**
 * CcEncoder.h - Library MIDI CC Controls.
 * Creates an encoder with two exclusive modes that sets an independent value
 * within a given range for each mode depending on bool passed in at update and
 * configures its button functionality.
 */

#ifndef CcEncoder_h
#define CcEncoder_h

#include "Arduino.h"

class CcEncoder {
  public:
    CcEncoder(
      int pin1,           // Leg 1 pin.
      int pin2,           // Leg 3 pin.
      int pin3,           // Button pin.
      int maxValueA,      // Max value for encoder in default mode.
      int minValueA,      // Min value for encoder in default mode.
      int defaultA,       // Default value in default mode.
      int maxValueB,      // Max value for encoder in alternate mode.
      int minValueB,      // Min value for encoder in alternate mode.
      int defaultB,       // Default value in alternate mode.
      int debounceMS,     // Debounce time in milliseconds.
      char ccName[]       // Name for control.
    );
    void begin();         // Initialize. Call from setup.
    void update(bool alternateValue); // Process. Call from loop.
    int read(bool alternateValue); // Get the current value of encoder.
    int showValue();      // Whether or not to display value.
    int getStartTime();   // Return time last changed.
    void setValue(int value, bool alternateValue); // Sets the encoder value.
  private:
    int _pin1;            // Leg 1 pin.
    int _pin2;            // Leg 3 pin.
    int _pin3;            // Button pin.
    int _maxValueA;       // Max value for encoder in default mode.
    int _minValueA;       // Min value for encoder in default mode.
    int _valueA;          // Encoder value in default mode.
    int _maxValueB;       // Max value for encoder in alternate mode.
    int _minValueB;       // Min value for encoder in alternate mode.
    int _valueB;          // Encoder value in alternate mode.
    char * _ccName;       // Name for control.
    QuadEncoder _encoder; // Encoder.
    Bounce _btn;          // Debounced button.
    int _displayValue = false;    // Whether or not to show on display.
    long _displayTimeout = 4000;  // How long to show on display.
    long _startTime = 0;          // Time when started showing on display.
};

#endif
