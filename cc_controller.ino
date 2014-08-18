#include <Bounce.h>
#include <MIDI.h>

const int midiChannel = 1;
const long debounceMS = 500;

struct ccControl {
  int btnPin;
  int ledPin;
  int state;
  int previous;
  int stateOn;
  int stateOff;
  int cc;
  char name[10];
  int reading;
};

ccControl local = { 5, 21, HIGH, LOW, 127, 0, 122, "local" };
ccControl arp = { 6, 13, HIGH, LOW, 0, 127, 117, "arp hold" };

Bounce localBtn = Bounce(local.btnPin, debounceMS);
Bounce arpBtn = Bounce(arp.btnPin, debounceMS);

char buffer[60];


void setup() {
  MIDI.begin();
  initUI(local);
  initUI(arp);
}


void loop() {

  localBtn.update();
  arpBtn.update();

  local.reading = localBtn.read();
  processButtonState(localBtn, local);

  arp.reading = arpBtn.read();
  processButtonState(arpBtn, arp);

}


void initUI(struct ccControl control) {
  pinMode(control.btnPin, INPUT_PULLUP);
  pinMode(control.ledPin, OUTPUT);
  digitalWrite(control.ledPin, control.previous);
}


void processButtonState(Bounce btn, struct ccControl &control) {
  if (control.reading != control.previous && btn.fallingEdge()) {
    if (control.state == HIGH) {
      usbMIDI.sendControlChange(control.cc, control.stateOff, midiChannel);
      MIDI.sendControlChange(control.cc, control.stateOff, midiChannel);
      // sprintf(buffer, "Toggle %s led on: %x", control.name, control.stateOff);
    } else {
      usbMIDI.sendControlChange(control.cc, control.stateOn, midiChannel);
      MIDI.sendControlChange(control.cc, control.stateOn, midiChannel);
      // sprintf(buffer, "Toggle %s led off: %x", control.name, control.stateOn);
    }
    // Serial.println(buffer);
    digitalWrite(control.ledPin, control.state);

    control.state = !control.state;
  }
  control.previous = control.reading;
}
