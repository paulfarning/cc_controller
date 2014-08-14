struct ccControl {
  int btnPin;
  int ledPin;
  int state;
  int previous;
  int stateOn;
  int stateOff;
  char name[10];
  int reading;
};

ccControl local = { 5, 21, HIGH, LOW, 127, 0, "local" };
ccControl arp = { 6, 13, HIGH, LOW, 0, 127, "arp hold" };

long time = 0;
long debounce = 500;

char buffer[60];


void initUI(struct ccControl control) {
  pinMode(control.btnPin, INPUT_PULLUP);
  pinMode(control.ledPin, OUTPUT);
  sprintf(buffer, "Init MIDI value: %x", control.stateOn);
  Serial.println(buffer);
  digitalWrite(control.ledPin, control.previous);
}


void processButtonState(struct ccControl &control) {
  if (control.reading == HIGH && control.previous == LOW && millis() - time > debounce) {
    if (control.state == HIGH) {
      sprintf(buffer, "Toggle %s led on: %x", control.name, control.stateOff);
      Serial.println(buffer);
    } else {
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


void setup() {
  initUI(local);
  initUI(arp);
}


void loop() {

  local.reading = digitalRead(local.btnPin);
  processButtonState(local);

  arp.reading = digitalRead(arp.btnPin);
  processButtonState(arp);

}
