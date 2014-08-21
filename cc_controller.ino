#include <Bounce.h>
#include <MIDI.h>
#include "CcButton.h"

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

const int midiChannel = 1;
const long debounceMS = 500;

char buffer[60];

CcButton controls[] = {
  CcButton(5, 21, HIGH, LOW, 127, 0, 122, "local"),
  CcButton(6, 13, HIGH, LOW, 0, 127, 117, "arp hold"),
  CcButton(7, 22, HIGH, LOW, 127, 0, 110, "lfo"),
  CcButton(8, 23, HIGH, LOW, 0, 127, 109, "legato")
};


void setup() {
  MIDI.begin();

  for(int i = 0; i < ARRAY_SIZE(controls); i++) {
    controls[i].begin();
  }

}


void loop() {

  for(int i = 0; i < ARRAY_SIZE(controls); i++) {
    controls[i].update();
  }

}


