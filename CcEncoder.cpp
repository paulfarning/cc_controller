/*
  CcEncoder.cpp - Library MIDI CC Controls.
  Creates an encoder that sets a value between 0 and 127.
*/

#include "Arduino.h"
#include "QuadEncoder.h"
#include "CcEncoder.h"

CcEncoder::CcEncoder(
  int pin1,
  int pin2,
  int maxValue,
  int minValue,
  char ccName[]
) : _encoder(pin1, pin2) {
  _pin1 = pin1;
  _pin2 = pin2;
  _maxValue = maxValue;
  _minValue = minValue;
  _ccName = ccName;
  _value = 0;
  _displayValue = false;
  _displayTimeout = 4000;
  _startTime = 0;
}


void CcEncoder::begin() {

}


void CcEncoder::update() {

  int move = _encoder.tick();

  if (move == '>' || move == '<') {
    if (move == '>') {
      _value = _value < _maxValue ? _value + 1 : _minValue;
    } else if (move == '<') {
      _value = _value > _minValue ? _value - 1 : _maxValue;
    }
    _startTime = millis();
    _displayValue = true;
  } else {
    if (millis() - _startTime > _displayTimeout) {
      _displayValue = false;
    }
  }

}

int CcEncoder::read() {
  return _value;
}

int CcEncoder::showValue() {
  return _displayValue;
}

int CcEncoder::getStartTime() {
  return _startTime;
}
