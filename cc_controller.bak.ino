#include <Bounce.h>
#include <MIDI.h>

const int midiChannel = 1;
const long debounceMS = 500;

char buffer[60];

struct ccControl {
  int btnPin;     // pin button is on
  int ledPin;     // pin led is on
  int state;      // default state
  int previous;   // previous state
  int stateOn;    // default cc value
  int stateOff;   // toggled cc value (led on)
  int cc;         // cc data number
  char name[10];  // name for debug loggin
  // Bounce btn;     // button
  int reading;    // current state, set by bounce read()
};

ccControl controls[] = {
  { 5, 21, HIGH, LOW, 127, 0, 122, "local" },
  { 6, 13, HIGH, LOW, 0, 127, 117, "arp hold" },
  { 7, 22, HIGH, LOW, 127, 0, 110, "lfo" },
  { 8, 23, HIGH, LOW, 0, 127, 109, "legato" }
};

// Bounce controls[0].btn = Bounce(controls[0].btnPin, debounceMS);
// Bounce controls[1].btn = Bounce(controls[1].btnPin, debounceMS);
// Bounce controls[2].btn = Bounce(controls[2].btnPin, debounceMS);
// Bounce controls[3].btn = Bounce(controls[3].btnPin, debounceMS);


// ccControl local = { 5, 21, HIGH, LOW, 127, 0, 122, "local", Bounce(local.btnPin, debounceMS) };
// ccControl arp = { 6, 13, HIGH, LOW, 0, 127, 117, "arp hold", Bounce(arp.btnPin, debounceMS) };
// ccControl lfo = { 7, 22, HIGH, LOW, 127, 0, 110, "lfo", Bounce(lfo.btnPin, debounceMS) };
// ccControl legato = { 8, 23, HIGH, LOW, 0, 127, 109, "legato", Bounce(legato.btnPin, debounceMS) };

// Bounce localBtn = Bounce(local.btnPin, debounceMS);
// Bounce arpBtn = Bounce(arp.btnPin, debounceMS);
// Bounce lfoBtn = Bounce(lfo.btnPin, debounceMS);
// Bounce legatoBtn = Bounce(legato.btnPin, debounceMS);


void setup() {
  MIDI.begin();
  for(int i = 0; i < sizeof(controls) - 1; i++) {
    Bounce controls[i].btn = Bounce(controls[i].btnPin, debounceMS);
    initUI(controls[i]);
  }
  // initUI(local);
  // initUI(arp);
  // initUI(lfo);
  // initUI(legato);
}


void loop() {

  for(int i = 0; i < sizeof(controls) - 1; i++) {
    controls[i].btn.update();
    controls[i].reading = controls[i].btn.read();
    processButtonState(controls[i]);
  }

  // localBtn.update();
  // arpBtn.update();
  // lfoBtn.update();
  // legatoBtn.update();

  // local.reading = localBtn.read();
  // processButtonState(localBtn, local);

  // arp.reading = arpBtn.read();
  // processButtonState(arpBtn, arp);

  // lfo.reading = lfoBtn.read();
  // processButtonState(lfoBtn, lfo);

  // legato.reading = legatoBtn.read();
  // processButtonState(legatoBtn, legato);

}


void initUI(struct ccControl control) {
  pinMode(control.btnPin, INPUT_PULLUP);
  pinMode(control.ledPin, OUTPUT);
  digitalWrite(control.ledPin, control.previous);
}


// void processButtonState(Bounce btn, struct ccControl &control) {
void processButtonState(struct ccControl &control) {
  if (control.reading != control.previous && control.btn.fallingEdge()) {
    if (control.state == HIGH) {
      usbMIDI.sendControlChange(control.cc, control.stateOff, midiChannel);
      MIDI.sendControlChange(control.cc, control.stateOff, midiChannel);
      sprintf(buffer, "Toggle %s led on: %x", control.name, control.stateOff);
    } else {
      usbMIDI.sendControlChange(control.cc, control.stateOn, midiChannel);
      MIDI.sendControlChange(control.cc, control.stateOn, midiChannel);
      sprintf(buffer, "Toggle %s led off: %x", control.name, control.stateOn);
    }
    Serial.println(buffer);
    digitalWrite(control.ledPin, control.state);

    control.state = !control.state;
  }
  control.previous = control.reading;
}
