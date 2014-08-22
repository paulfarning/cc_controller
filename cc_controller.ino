#include <Bounce.h>
#include <MIDI.h>
#include <QuadEncoder.h>
#include "CcButton.h"
#include "CcEncoder.h"

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

const int midiChannel = 1;
const int debounceMS = 500;
const int sendEncodersBtnPin = 9;

CcButton CcButtons[] = {
  CcButton(5, 21, HIGH, LOW, 127, 0, 122, midiChannel, debounceMS, "Local"),
  CcButton(6, 13, HIGH, LOW, 0, 127, 117, midiChannel, debounceMS, "Arp Hold"),
  CcButton(7, 22, HIGH, LOW, 127, 0, 110, midiChannel, debounceMS, "LFO"),
  CcButton(8, 23, HIGH, LOW, 0, 127, 109, midiChannel, debounceMS, "Legato")
};


CcEncoder CcEncoders[] = {
  CcEncoder(15, 14, 127, 0, "CC Number"),
  CcEncoder(17, 16, 127, 0, "CC Value")
};

Bounce sendEncodersBtn = Bounce(sendEncodersBtnPin, debounceMS);

// char buffer[60];


void setup() {
  Serial.begin(9600);
  MIDI.begin();
  pinMode(9, INPUT_PULLUP);

  for(int i = 0; i < ARRAY_SIZE(CcButtons); i++) {
    CcButtons[i].begin();
  }

}


void loop() {

  if (sendEncodersBtn.update()) {
    if (sendEncodersBtn.fallingEdge()) {
      MIDI.sendControlChange(CcEncoders[0].read(), 108, midiChannel);
      Serial.print(CcEncoders[0].read());
    }
  }



  for(int i = 0; i < ARRAY_SIZE(CcEncoders); i++) {
    CcEncoders[i].update();
  }


  for(int i = 0; i < ARRAY_SIZE(CcButtons); i++) {
    CcButtons[i].update();
  }

}


