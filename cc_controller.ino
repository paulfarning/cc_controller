#define HWSERIAL Serial1

const int channel = 1;

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

long time = 0;
long debounce = 500;

char buffer[60];


void setup() {
  Serial.begin(31250);
  HWSERIAL.begin(31250);
  initUI(local);
  initUI(arp);
}


void loop() {

  local.reading = digitalRead(local.btnPin);
  processButtonState(local);

  arp.reading = digitalRead(arp.btnPin);
  processButtonState(arp);

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
  Serial.println(cmd, DEC);
  Serial.println(data1, DEC);
  Serial.println(data2, DEC);
}


void processButtonState(struct ccControl &control) {
  if (control.reading == HIGH && control.previous == LOW && millis() - time > debounce) {
    if (control.state == HIGH) {
      usbMIDI.sendControlChange(control.cc, control.stateOff, channel);
      sendCC(control.cc, control.stateOff, control.stateOff);
      sprintf(buffer, "Toggle %s led on: %x", control.name, control.stateOff);
      Serial.println(buffer);
    } else {
      usbMIDI.sendControlChange(control.cc, control.stateOn, channel);
      sendCC(control.cc, control.stateOn, control.stateOn);
      sprintf(buffer, "Toggle %s led off: %x", control.name, control.stateOn);
      Serial.println(buffer);
    }
    digitalWrite(control.ledPin, control.state);

    if (control.state == HIGH)
      control.state = LOW;
    else
      control.state = HIGH;

    time = millis();
  }
  control.previous = control.reading;
}
