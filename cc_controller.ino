/**
 * CC Controller
 *
 * MIDI controller with 2 encoders, a 4 digit display, 5 buttons and 4 LEDs.
 * Four of the buttons toggle specific CC messages and a corresponding LED. The
 * encoders can be used to select a CC number and value to send with the fifth
 * button. The display shows the midi channel on startup and the encoder values
 * when editing or when pushed.
 *
 * Built for Teensy 3.1.
 *
 * Created 10.20.2014
 * By Paul Farning
 *
 * https://github.com/paulfarning/cc_controller
 */



#include <Bounce.h>
#include <MIDI.h>
#include <QuadEncoder.h>
#include <SevSeg.h>
#include "CcButton.h"
#include "CcEncoder.h"
#include "MomentaryButton.h"

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

const int debounceMS = 500;
const int holdMS = 2000;
const int sendEncodersBtnPin = 2;
const int initDelay = 4000;

int encoderToDisplay = -1;
int midiChannel = 1;
unsigned long currentMillis;

SevSeg bubbleDisplay;

CcButton CcButtons[] = {
  CcButton(3, 7, HIGH, LOW, 127, 0, 122, debounceMS, "Local"),
  CcButton(4, 8, HIGH, LOW, 0, 127, 117, debounceMS, "Arp Hold"),
  CcButton(5, 9, HIGH, LOW, 127, 0, 110, debounceMS, "LFO"),
  CcButton(6, 10, HIGH, LOW, 0, 127, 109, debounceMS, "Legato")
};

CcEncoder CcEncoders[] = {
  CcEncoder(28, 27, 26, 127, 0, debounceMS, "CC Number"), // Left encoder
  CcEncoder(31, 30, 29, 127, 0, debounceMS, "CC Value") // Right encoder
};

// Bounce sendEncodersBtn = Bounce(sendEncodersBtnPin, debounceMS);

// MomentaryButton button(sendEncodersBtnPin);

// int buttonVal;
// int btnUpTime;
// int buttonLast;
// int ignoreUp;
// unsigned long btnDnTime;



unsigned long keyPrevMillis = 0;
const unsigned long keySampleIntervalMs = 25;
byte longKeyPressCountMax = 80;    // 80 * 25 = 2000 ms
byte longKeyPressCount = 0;
byte prevKeyState = HIGH;         // button is active low


/**
 * Initializes inputs and outputs.
 */
void setup() {
  Serial.begin(9600);
  MIDI.begin();
  pinMode(sendEncodersBtnPin, INPUT_PULLUP);

  for(int i = 0; i < ARRAY_SIZE(CcButtons); i++) {
    CcButtons[i].begin();
  }

  for(int i = 0; i < ARRAY_SIZE(CcEncoders); i++) {
    CcEncoders[i].begin();
  }

  // button.setup();

  setupDisplay();

}


/**
 * Computes changes on all inputs and outputs.
 */
void loop() {

  currentMillis = millis();

  // Display midi channel on startup.
  if (currentMillis < initDelay && encoderToDisplay == -1) {
    writeToDisplay(midiChannel, 'c');
  }

  // Send encoders if button pushed.
  // if (sendEncodersBtn.update()) {
  //   if (sendEncodersBtn.fallingEdge()) {
  //     Serial.println("clicked");
  //     usbMIDI.sendControlChange(
  //       CcEncoders[1].read(),
  //       CcEncoders[0].read(),
  //       midiChannel
  //     );
  //     MIDI.sendControlChange(
  //       CcEncoders[1].read(),
  //       CcEncoders[0].read(),
  //       midiChannel
  //     );
  //   }
  // }

  // button.check();
  // if (button.wasClicked()) {
  //   Serial.println("clicked");
  // } else if (button.wasHeld()) {
  //   Serial.println("held");
  // }

  // if (sendEncodersBtn.update()) {
  //   button.check();
  //   if (button.wasClicked()) {
  //     if (sendEncodersBtn.fallingEdge()) {
  //       Serial.println("clicked");
  //       usbMIDI.sendControlChange(
  //         CcEncoders[1].read(),
  //         CcEncoders[0].read(),
  //         midiChannel
  //       );
  //       MIDI.sendControlChange(
  //         CcEncoders[1].read(),
  //         CcEncoders[0].read(),
  //         midiChannel
  //       );
  //     }
  //   } else if (button.wasHeld()) {
  //    if (sendEncodersBtn.risingEdge()) {
  //       Serial.println("held");

  //       usbMIDI.sendControlChange(
  //         CcEncoders[1].read(),
  //         CcEncoders[0].read(),
  //         midiChannel
  //       );
  //       MIDI.sendControlChange(
  //         CcEncoders[1].read(),
  //         CcEncoders[0].read(),
  //         midiChannel
  //       );
  //     }
  //   }
  // }

  // checkEncodersButton();


    if (millis() - keyPrevMillis >= keySampleIntervalMs) {
        keyPrevMillis = millis();

        byte currKeyState = digitalRead(sendEncodersBtnPin);

        if ((prevKeyState == HIGH) && (currKeyState == LOW)) {
            keyPress();
        }
        else if ((prevKeyState == LOW) && (currKeyState == HIGH)) {
            keyRelease();
        }
        else if (currKeyState == LOW) {
            longKeyPressCount++;
        }

        prevKeyState = currKeyState;
    }





  // Update CC buttons.
  for (int i = 0; i < ARRAY_SIZE(CcButtons); i++) {
    CcButtons[i].update(midiChannel);
  }

  // Update encoders.
  for (int i = 0; i < ARRAY_SIZE(CcEncoders); i++) {
    CcEncoders[i].update();
  }

  // Update display with encoder value if changed.
  encoderToDisplay = getEncoderToDisplay();
  if (encoderToDisplay != -1) {
    writeToDisplay(CcEncoders[encoderToDisplay].read(), ' ');
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

// void checkEncodersButton() {
//   // Read the state of the button
//   buttonVal = digitalRead(sendEncodersBtnPin);

//   // // Test for button pressed and store the down time
//   if (buttonVal == LOW && buttonLast == HIGH && (millis() - btnUpTime) > long(debounceMS)) {
//     btnDnTime = millis();

//     Serial.println(btnDnTime);
//   }

//   // // Test for button release and store the up time

//   if (buttonVal == HIGH && buttonLast == LOW && (millis() - btnDnTime) > long(debounceMS)) {
//     Serial.println("here");
//     if (ignoreUp == false) {

//     Serial.println("in");
//       event1();
//     } else {
//       ignoreUp = false;

//     Serial.println("out");
//     }
//     btnUpTime = millis();

//     Serial.println("done");
//   }

//   // // Test for button held down for longer than the hold time
//   if (buttonVal == LOW && (millis() - btnDnTime) > long(holdMS)) {
//     event2();
//     ignoreUp = true;
//     btnDnTime = millis();
//   }

//   buttonLast = buttonVal;
// }


// void event1() {
//   Serial.println("clicked");
// }

// void event2() {
//   Serial.println("held");
// }
//
//
//
//



// called when button is kept pressed for less than 2 seconds
void shortKeyPress() {
    Serial.println("short");
}


// called when button is kept pressed for more than 2 seconds
void longKeyPress() {
    Serial.println("long");
}


// called when key goes from not pressed to pressed
void keyPress() {
    Serial.println("key press");
    longKeyPressCount = 0;
}


// called when key goes from pressed to not pressed
void keyRelease() {
    Serial.println("key release");

    if (longKeyPressCount >= longKeyPressCountMax) {
        longKeyPress();
    }
    else {
        shortKeyPress();
    }
}


