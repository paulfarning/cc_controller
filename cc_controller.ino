#include <Bounce.h>
#define HWSERIAL Serial1

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
  Serial.begin(31250);
  HWSERIAL.begin(31250);
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
  sprintf(buffer, "Init MIDI value: %x", control.stateOn);
  Serial.println(buffer);
  digitalWrite(control.ledPin, control.previous);
}


void sendCC(int cmd, int data1, int data2) {
  HWSERIAL.write(cmd);
  HWSERIAL.write(data1);
  HWSERIAL.write(data2);

  // Serial.println(cmd, DEC);
  // Serial.println(data1, DEC);
  // Serial.println(data2, DEC);
}


void processButtonState(Bounce btn, struct ccControl &control) {
  if (control.reading != control.previous && btn.fallingEdge()) {
    if (control.state == HIGH) {
      usbMIDI.sendControlChange(control.cc, control.stateOff, midiChannel);
      sendCC(control.cc, control.stateOff, control.stateOff);
      sprintf(buffer, "Toggle %s led on: %x", control.name, control.stateOff);
    } else {
      usbMIDI.sendControlChange(control.cc, control.stateOn, midiChannel);
      sendCC(control.cc, control.stateOn, control.stateOn);
      sprintf(buffer, "Toggle %s led off: %x", control.name, control.stateOn);
    }
    // Serial.println(buffer);
    digitalWrite(control.ledPin, control.state);

    control.state = !control.state;
  }
  control.previous = control.reading;
}
