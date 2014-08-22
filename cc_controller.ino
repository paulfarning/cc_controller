#include <Bounce.h>
#include <MIDI.h>
#include "CcButton.h"

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

const int midiChannel = 1;
const long debounceMS = 500;

// char buffer[60];


CcButton CcButtons[] = {
  CcButton(5, 21, HIGH, LOW, 127, 0, 122, midiChannel, debounceMS, "Local"),
  CcButton(6, 13, HIGH, LOW, 0, 127, 117, midiChannel, debounceMS, "Arp Hold"),
  CcButton(7, 22, HIGH, LOW, 127, 0, 110, midiChannel, debounceMS, "LFO"),
  CcButton(8, 23, HIGH, LOW, 0, 127, 109, midiChannel, debounceMS, "Legato")
};


// Encoder Encoders[] = {
//   Encoder(9, "CC Number"),
//   Encoder(10, "CC Value")
// };


void setup() {
  MIDI.begin();

  for(int i = 0; i < ARRAY_SIZE(CcButtons); i++) {
    CcButtons[i].begin();
  }

}


void loop() {

  for(int i = 0; i < ARRAY_SIZE(CcButtons); i++) {
    CcButtons[i].update();
  }

}


