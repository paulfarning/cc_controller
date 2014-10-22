CC Controller
=============

MIDI controller with 2 encoders, a 4 segment display, 5 buttons and 4 LEDs.
Four of the buttons toggle specific CC messages and a corresponding LED. The
encoders can be used to select a CC number and value to send with the fifth
button. The display shows the midi channel on startup and the encoder values
when editing or when pushed.

Built for Teensy 3.1.

Required libraries:
-------------------
1. [Bounce 1](https://github.com/thomasfredericks/Bounce-Arduino-Wiring)
2. [MIDI](https://github.com/FortySevenEffects/arduino_midi_library)
3. [QuadEncoder](https://github.com/medecau/QuadEncoder)
4. [SevSeg](https://github.com/sparkfun/SevSeg)
