# CC Controller

MIDI controller with 2 encoders, a 4 segment display, 5 buttons and 4 LEDs.
Four of the buttons toggle specific CC messages and a corresponding LED. The
encoders can be used to select a CC number and value to send with the fifth
button. The display shows the midi channel on startup and the encoder values
when editing or when pushed.

Built on [Teensy 3.1](https://www.pjrc.com/teensy/).

## Required libraries:
1. [Bounce 1](https://github.com/thomasfredericks/Bounce-Arduino-Wiring)
2. [MIDI v4.2](https://github.com/FortySevenEffects/arduino_midi_library)
3. [QuadEncoder](https://github.com/medecau/QuadEncoder)
4. [SevSeg](https://github.com/sparkfun/SevSeg)

## Teensy pinout:
**Pin 1:** Midi out  
**Pin 2:** Encoder send button  
**Pins 3-6:** CC toggle buttons  
**Pins 7-10:** CC LEDs  
**Pins 11-12 + 14-23:** Display  
**Pins 26-28:** Encoder 1 (button, left, right)  
**Pins 29-31:** Encoder 2 (button, left, right)  

## Components
Here are some of the components I used. Most are also available from other
vendors. I bought from these vendors because despite my lack of hardware
knowledge their websites made it easy to browse and understand what I was
getting. They all ship fast and for a rate that keeps small orders affordable.
Thank you to these folks, I'm happy to support them.

1. [PJRC Teensy 3.1](https://www.pjrc.com/teensy/)
2. [Sparkfun 7 segment bubble display](https://www.sparkfun.com/products/12710)
3. [Sparkfun Mini Pushbutton](https://www.sparkfun.com/products/97)
4. [Adafruit Rotary Encoders](http://www.adafruit.com/products/377)
5. [Adafruit Diffused Red 3mm LED](http://www.adafruit.com/product/777)

And [URS Electronics](http://www.ursele.com/) brick and mortar store in
Portland, Oregon.

## CC parameters
This was built for toggling parameters of an Arturia Minibrute. When used with a
Minbrute, the four buttons toggle Local On/Off, Arpeggiator Hold, LFO Retrigger
and Envelope Legato. The CC number and values for each button can be changed in
cc_controller.ino. The above hardware configuration leaves room on the Teensy
for a couple more buttons. To add more consider a shift register.

The encoders can be used to set any valid CC number and value combination to
provide some flexibility beyond the four hardcoded buttons.

## State
There is no midi in. No midi data is read from the connected device. The
state of the LEDs displayed may not match the state of the device when first
connected. Toggle through the buttons to ensure they are reset and accurate.

## Device usage
When powered on the display shows the midi channel to be used for sending
messages.

Push a dedicated CC button to send a midi message to toggle its value and
toggle the corresponding LED.

Encoder 1 selects the CC number to be sent. Encoder 2 selects the value to set
for that number. Each has a range of 0 to 127. The encoders are also buttons.
Press them to display their current value.

Press the additional push button to send the midi message with the current
number and value pair of the encoders.

## Example build
The extra jacks and led are for a
[Dtronics midi to trigger kit](http://www.synthcity.nl/midi/midi-to-trigger-interface.html).  
![Front](controller/front.jpg?raw=true)
![back](controller/back.jpg?raw=true)
