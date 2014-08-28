#include <Bounce.h>
#include <MIDI.h>
#include <QuadEncoder.h>
#include "SevSeg.h"
#include "CcButton.h"
#include "CcEncoder.h"

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

const int midiChannel = 1;
const int debounceMS = 500;
const int sendEncodersBtnPin = 9;
const int initDelay = 4000;

// long encoderTimeouts[] = { 0, 0 };

unsigned long currentMillis;


// long previousMillis = 0;
// long interval = 4000;

SevSeg bubbleDisplay;

// CcButton CcButtons[] = {
//   CcButton(5, 21, HIGH, LOW, 127, 0, 122, midiChannel, debounceMS, "Local"),
//   CcButton(6, 13, HIGH, LOW, 0, 127, 117, midiChannel, debounceMS, "Arp Hold"),
//   CcButton(7, 22, HIGH, LOW, 127, 0, 110, midiChannel, debounceMS, "LFO"),
//   CcButton(8, 23, HIGH, LOW, 0, 127, 109, midiChannel, debounceMS, "Legato")
// };

CcEncoder CcEncoders[] = {
  CcEncoder(7, 6, 127, 0, "CC Number"),
  CcEncoder(9, 8, 127, 0, "CC Value")
};

Bounce sendEncodersBtn = Bounce(sendEncodersBtnPin, debounceMS);



// char buffer[60];


void setup() {
  Serial.begin(9600);
  MIDI.begin();
  pinMode(9, INPUT_PULLUP);

  // for(int i = 0; i < ARRAY_SIZE(CcButtons); i++) {
  //   CcButtons[i].begin();
  // }

  setupDisplay();


}


void loop() {

  currentMillis = millis();

  if (currentMillis < initDelay) {
    writeToDisplay(midiChannel, 'c');
  }


  if (sendEncodersBtn.update()) {
    if (sendEncodersBtn.fallingEdge()) {
      MIDI.sendControlChange(CcEncoders[0].read(), 108, midiChannel);
      Serial.print(CcEncoders[0].read());
    }
  }

  // for (int i = 0; i < ARRAY_SIZE(CcButtons); i++) {
  //   CcButtons[i].update();
  // }



  for (int i = 0; i < ARRAY_SIZE(CcEncoders); i++) {
    CcEncoders[i].update();
  }


  int encoderToDisplay = getEncoderToDisplay();
  if (encoderToDisplay != -1) {
    writeToDisplay(CcEncoders[encoderToDisplay].read(), ' ');
  }

}


void setupDisplay() {
  int displayType = COMMON_CATHODE;
  int numberOfDigits = 4;

  // Declare what pins are connected to the GND pins (cathodes)
  int digit1 = 23; //Pin 1
  int digit2 = 22; //Pin 10
  int digit3 = 21; //Pin 4
  int digit4 = 20; //Pin 6

  // Declare what pins are connected to the segments (anodes)
  int segA = 19; //Pin 12
  int segB = 18; //Pin 11
  int segC = 17; //Pin 3
  int segD = 16; //Pin 8
  int segE = 15; //Pin 2
  int segF = 14; //Pin 9
  int segG = 12; //Pin 7
  int segDP= 11; //Pin 5

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
  }
  return encoderIndex;

}


