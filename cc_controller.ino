/**
 * CC Controller
 *
 * MIDI controller with 2 encoders, a 4 digit display, 5 buttons and 4 LEDs.
 * Four of the buttons toggle specific CC messages and a corresponding LED. The
 * encoders can be used to select a CC number and value to send with the fifth
 * button. They also have an alternate mode for changing the midi channel
 * enabled by holding the fifth button for >2 seconds. The display shows the
 * midi channel on startup and when in alternate mode and the encoder values
 * when editing or when they are pushed.
 *
 * Built for Teensy 3.1.
 *
 * Created 10.20.2014
 * Updated 11.16.2014
 *
 * https://github.com/paulfarning/cc_controller
 */



#include <Bounce.h>
#include <EEPROM.h>
#include <MIDI.h>
#include <QuadEncoder.h>
#include <SevSeg.h>
#include "CcButton.h"
#include "CcEncoder.h"

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

const int debounceMS = 500;
const int holdMS = 2000;
const int initDelay = 4000;
const int sendEncodersBtnPin = 2;
const int midiChannelAddress = 0;

int midiChannel = 1;
int value;

int encoderToDisplay = -1;
bool changeMidiChannelMode = false;
unsigned long currentMillis;

SevSeg bubbleDisplay;

CcButton CcButtons[] = {
  CcButton(3, 7, HIGH, LOW, 127, 0, 122, debounceMS, "Local"),
  CcButton(4, 8, HIGH, LOW, 0, 127, 117, debounceMS, "Arp Hold"),
  CcButton(5, 9, HIGH, LOW, 127, 0, 110, debounceMS, "LFO"),
  CcButton(6, 10, HIGH, LOW, 0, 127, 109, debounceMS, "Legato")
};

CcEncoder CcEncoders[] = {
  CcEncoder(31, 30, 29, 127, 0, 0, 16, 1, midiChannel, debounceMS, "CC Number"), // Left encoder
  CcEncoder(28, 27, 26, 127, 0, 0, 0, 0, 0, debounceMS, "CC Value") // Right encoder
};

// changeMidiChannelMode config and storage.
unsigned long keyPrevMillis = 0;
const unsigned long keySampleIntervalMs = 25;
byte longKeyPressCountMax = holdMS / keySampleIntervalMs;
byte longKeyPressCount = 0;
byte prevKeyState = HIGH;


/**
 * Initializes inputs and outputs.
 */
void setup() {
  Serial.begin(9600);
  MIDI.begin();
  pinMode(sendEncodersBtnPin, INPUT_PULLUP);

  value = EEPROM.read(midiChannelAddress);

  if (value > 0 && value < 17) {
    midiChannel = value;
  }

  for(int i = 0; i < ARRAY_SIZE(CcButtons); i++) {
    CcButtons[i].begin();
  }

  for(int i = 0; i < ARRAY_SIZE(CcEncoders); i++) {
    CcEncoders[i].begin();
  }

  CcEncoders[0].setValue(midiChannel, true);

  setupDisplay();

}


/**
 * Computes changes on all inputs and outputs.
 */
void loop() {

  currentMillis = millis();

  // Display midi channel on startup or when in change channel mode.
  if ((currentMillis < initDelay && encoderToDisplay == -1) ||
      changeMidiChannelMode) {
    writeToDisplay(midiChannel, 'c');
  }

  // Update sendEncodersBtn mode and state.
  // If short press, evaluate based on changeMidiChannelMode value.
  // If held, enter changeMidiChannelMode and reset on next short press.
  // Base on code from:
  // http://forum.arduino.cc/index.php?PHPSESSID=u97ebbav3ubd4qkvkkbo5k7bo0&topic=140123.msg1052582#msg1052582
  if (currentMillis - keyPrevMillis >= keySampleIntervalMs) {
    keyPrevMillis = currentMillis;

    byte currKeyState = digitalRead(sendEncodersBtnPin);

    if ((prevKeyState == HIGH) && (currKeyState == LOW)) {

      longKeyPressCount = 0;

    } else if ((prevKeyState == LOW) && (currKeyState == HIGH)) {

      if (longKeyPressCount < longKeyPressCountMax) {
        if (changeMidiChannelMode) {
          sendMidiChannelChange();
        } else {
          sendEncoderPair();
        }
        changeMidiChannelMode = false;
      }

    } else if (currKeyState == LOW) {
      longKeyPressCount++;

      if (longKeyPressCount >= longKeyPressCountMax) {
        changeMidiChannelMode = true;
      }
    }

    prevKeyState = currKeyState;
  }

  // Update CC buttons.
  for (int i = 0; i < ARRAY_SIZE(CcButtons); i++) {
    CcButtons[i].update(midiChannel);
  }

  // Update encoders.
  for (int i = 0; i < ARRAY_SIZE(CcEncoders); i++) {
    CcEncoders[i].update(changeMidiChannelMode);
    if (changeMidiChannelMode) {
      midiChannel = CcEncoders[0].read(true);
    }
  }

  // Update display with encoder value if changed.
  encoderToDisplay = getEncoderToDisplay();
  if (encoderToDisplay != -1 && !changeMidiChannelMode) {
    writeToDisplay(CcEncoders[encoderToDisplay].read(false), ' ');
  }

}


/**
 * Initializes the 4 digit, 7 segment display.
 */
void setupDisplay() {
  int displayType = COMMON_CATHODE;
  int numberOfDigits = 4;

  // Declare what pins are connected to the GND pins (cathodes)
  int digit1 = 11; // Display pin 1
  int digit2 = 12; // Display pin 10
  int digit3 = 14; // Display pin 4
  int digit4 = 15; // Display pin 6

  // Declare what pins are connected to the segments (anodes)
  int segA = 16; // Display pin 12
  int segB = 17; // Display pin 11
  int segC = 18; // Display pin 3
  int segD = 19; // Display pin 8
  int segE = 20; // Display pin 2
  int segF = 21; // Display pin 9
  int segG = 22; // Display pin 7
  int segDP= 23; // Display pin 5

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


/**
 * Writes message to display.
 * @param {int} value The value to display, max 3 digit number. Will be right aligned.
 * @param {int} prefix Single character prefix for display. Will be left aligned.
 */
void writeToDisplay(int value, int prefix) {
  char tempString[10];
  sprintf(tempString, "%4d", value);
  tempString[0] = prefix;
  bubbleDisplay.DisplayString(tempString, 0);
}


/**
 * Determines if an encoder value should be displayed and if so which one.
 * If encoder's showValue property is true, it is considered for display. The
 * encoder with most recent start time is displayed. Or none.
 * @return {int} Encoder to display or -1 to display none.
 */
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


/**
 * Sends the pair of values from encoders as CC message on usb and din.
 */
void sendEncoderPair() {
  usbMIDI.sendControlChange(
    CcEncoders[0].read(false),
    CcEncoders[1].read(false),
    midiChannel
  );
  MIDI.sendControlChange(
    CcEncoders[0].read(false),
    CcEncoders[1].read(false),
    midiChannel
  );
}


/**
 * Sets midi channel value.
 */
void sendMidiChannelChange() {
  midiChannel = CcEncoders[0].read(true);
  EEPROM.write(midiChannelAddress, midiChannel);
}
