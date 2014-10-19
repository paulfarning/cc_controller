#include <Bounce.h>
#include <MIDI.h>
#include <QuadEncoder.h>
#include "SevSeg.h"
#include "CcButton.h"
#include "CcEncoder.h"

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

const int midiChannel = 1;
const int debounceMS = 500;
const int sendEncodersBtnPin = 2;
const int initDelay = 4000;

unsigned long currentMillis;
int encoderToDisplay = -1;

SevSeg bubbleDisplay;

CcButton CcButtons[] = {
  CcButton(3, 7, HIGH, LOW, 127, 0, 122, midiChannel, debounceMS, "Local"),
  CcButton(4, 8, HIGH, LOW, 0, 127, 117, midiChannel, debounceMS, "Arp Hold"),
  CcButton(5, 9, HIGH, LOW, 127, 0, 110, midiChannel, debounceMS, "LFO"),
  CcButton(6, 10, HIGH, LOW, 0, 127, 109, midiChannel, debounceMS, "Legato")
};

CcEncoder CcEncoders[] = {
  CcEncoder(28, 27, 26, 127, 0, debounceMS, "CC Number"), // Left
  CcEncoder(31, 30, 29, 127, 0, debounceMS, "CC Value") // Right
};

Bounce sendEncodersBtn = Bounce(sendEncodersBtnPin, debounceMS);


void setup() {
  Serial.begin(9600);
  MIDI.begin();
  pinMode(4, INPUT_PULLUP);

  for(int i = 0; i < ARRAY_SIZE(CcButtons); i++) {
    CcButtons[i].begin();
  }

  for(int i = 0; i < ARRAY_SIZE(CcEncoders); i++) {
    CcEncoders[i].begin();
  }

  setupDisplay();

}


void loop() {

  currentMillis = millis();

  if (currentMillis < initDelay && encoderToDisplay == -1) {
    writeToDisplay(midiChannel, 'c');
  }


  if (sendEncodersBtn.update()) {
    if (sendEncodersBtn.fallingEdge()) {
      MIDI.sendControlChange(102, 108, midiChannel);
      Serial.print(CcEncoders[0].read());
    }
  }

  for (int i = 0; i < ARRAY_SIZE(CcButtons); i++) {
    CcButtons[i].update();
  }

  for (int i = 0; i < ARRAY_SIZE(CcEncoders); i++) {
    CcEncoders[i].update();
  }

  encoderToDisplay = getEncoderToDisplay();
  if (encoderToDisplay != -1) {
    writeToDisplay(CcEncoders[encoderToDisplay].read(), ' ');
  }

}


void setupDisplay() {
  int displayType = COMMON_CATHODE;
  int numberOfDigits = 4;

  // Declare what pins are connected to the GND pins (cathodes)
  int digit1 = 11; //Pin 1
  int digit2 = 12; //Pin 10
  int digit3 = 14; //Pin 4
  int digit4 = 15; //Pin 6

  // Declare what pins are connected to the segments (anodes)
  int segA = 16; // 19; //Pin 12
  int segB = 17; // 18; //Pin 11
  int segC = 18; // 17; //Pin 3
  int segD = 19; // 16; //Pin 8
  int segE = 20; // 15; //Pin 2
  int segF = 21; // 14; //Pin 9
  int segG = 22; // 12; //Pin 7
  int segDP= 23; // 11; //Pin 5

  bubbleDisplay.Begin(
    displayType,
    numberOfDigits,
    digit1,
    digit2,
    digit3,
    digit4,
    segA,
    segB,
    segC,
    segD,
    segE,
    segF,
    segG,
    segDP
  );
  bubbleDisplay.SetBrightness(100);
}


void writeToDisplay(int value, int prefix) {
  char tempString[10];
  sprintf(tempString, "%4d", value);
  tempString[0] = prefix;
  bubbleDisplay.DisplayString(tempString, 0);
}


int getEncoderToDisplay() {
  int encoderIndex = -1;
  unsigned long maxTime = 0;
  for (int i = 0; i < ARRAY_SIZE(CcEncoders); i++) {
    if (CcEncoders[i].showValue() && CcEncoders[i].getStartTime() > maxTime) {
      encoderIndex = i;
    }
    maxTime = CcEncoders[i].getStartTime();
  }
  return encoderIndex;
}
