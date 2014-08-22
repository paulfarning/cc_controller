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
}


void CcEncoder::begin() {

}


void CcEncoder::update() {

  int move = _encoder.tick();
  if (move == '>') {
    if (_value < _maxValue) {
      _value++;
    } else {
      _value = _minValue;
    }
    Serial.println("greater than");
    Serial.print(char(move));
    Serial.print(_value);
  } else if (move == '<') {
    if (_value > _minValue) {
      _value--;
    } else {
      _value = _maxValue;
    }
    Serial.println("less than");
    Serial.print(char(move));
    Serial.print(_value);
  }

}

int CcEncoder::read() {
  return _value;
}
