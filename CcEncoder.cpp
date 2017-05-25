/**
 * CcEncoder.cpp - Library MIDI CC Controls.
 * Creates an encoder with two exclusive modes that sets an independent value
 * within a given range for each mode depending on bool passed in at update and
 * configures its button functionality.
 */

#include "Arduino.h"
#include "Bounce.h"
#include "QuadEncoder.h"
#include "CcEncoder.h"

CcEncoder::CcEncoder(
  int pin1,
  int pin2,
  int pin3,
  int maxValueA,
  int minValueA,
  int defaultA,
  int maxValueB,
  int minValueB,
  int defaultB,
  int debounceMS,
  char ccName[]
) : _encoder(pin1, pin2), _btn(pin3, debounceMS) {
  _pin1 = pin1;
  _pin2 = pin2;
  _pin3 = pin3;
  _maxValueA = maxValueA;
  _minValueA = minValueA;
  _valueA = defaultA;
  _maxValueB = maxValueB;
  _minValueB = minValueB;
  _valueB = defaultB;
  _ccName = ccName;
  _displayValue = false;
  _displayTimeout = 4000;
  _startTime = 0;
}


/**
 * Initializes inputs and outputs.
 */
void CcEncoder::begin() {
  pinMode(_pin3, INPUT_PULLUP);
}


/**
 * Computes changes on inputs and outputs and sets values base on current state.
 * @param {bool} alternateValue Whether or not to use alternate value.
 */
void CcEncoder::update(bool alternateValue) {
  int move = _encoder.tick();

  if (move == '>' || move == '<') {
    if (move == '>') {
      if (alternateValue) {
        _valueB = _valueB < _maxValueB ? _valueB + 1 : _minValueB;
      } else {
        _valueA = _valueA < _maxValueA ? _valueA + 1 : _minValueA;
      }
    } else if (move == '<') {
      if (alternateValue) {
        _valueB = _valueB > _minValueB ? _valueB - 1 : _maxValueB;
      } else {
        _valueA = _valueA > _minValueA ? _valueA - 1 : _maxValueA;
      }
    }
    _startTime = millis();
    _displayValue = alternateValue ? false : true;
  } else {
    if (millis() - _startTime > _displayTimeout) {
      _displayValue = false;
    }
  }

  if (_btn.update()) {
    if (_btn.fallingEdge()) {
      _startTime = millis();
      _displayValue = true;
    } else {
      if (millis() - _startTime > _displayTimeout) {
        _displayValue = false;
      }
    }
  }
}


/**
 * Gets the encoder value.
 * @param {bool} alternateValue Whether or not to read alternate value.
 * @return {int} Encoder value.
 */
int CcEncoder::read(bool alternateValue) {
  return alternateValue ? _valueB : _valueA;
}


/**
 * Determines if encoder value is candidate to display.
 * @return {int} Display value.
 */
int CcEncoder::showValue() {
  return _displayValue;
}


/**
 * Gets time started to display value.
 * @return {long} Start time.
 */
int CcEncoder::getStartTime() {
  return _startTime;
}


/**
 * Sets the encoder current value.
 * @param {int} value The value to set the encoder to.
 * @param {bool} alternateValue Whether or not to set alternate value.
 */
void CcEncoder::setValue(int value, bool alternateValue) {
  if (alternateValue) {
    _valueB = value;
  } else {
    _valueA = value;
  }
}
