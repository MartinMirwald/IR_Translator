# IR_Translator

A versatile infrared signal translator device that captures IR signals from one remote control and outputs different IR signals.

## Overview

IR_Translator is designed to solve a common problem in home entertainment setups: controlling multiple devices with a single remote. The primary use case is enabling your set-top box remote to also control your TV's power, eliminating the need for multiple remotes.

## Features (Planned)

- **Learning Mode**: Press a button to enter training mode and pair input signals with output signals
- **Multiple Signal Pairs**: Store several IR signal pairs in memory
- **Automatic Translation**: Automatically send the paired output signal whenever a learned input signal is received
- **Visual Feedback**: LED indicator for training mode and signal reception/transmission
- **Quick Power Off**: Long button press sends the first stored signal (typically configured as power off)

## Hardware

A custom PCB has been developed for this project and will be uploaded soon. The board contains:
- IR receiver (VS1838)
- IR transmitter(s)
- ATtiny 412 microcontroller
- Input button
- Status LED
- Power management components

The PCB was designed using KiCad and the design files will be shared in this repository.

## Current Status

- ✅ PCB design completed
- ✅ Component testing with initial code
- 🔄 Full firmware in development
- 📅 PCB design files coming soon

## Planned Implementation

The final implementation will allow users to:
1. Press a button to enter learning mode
2. Point a remote at the receiver and press the button to be captured (e.g., power button)
3. Point the target device's remote at the receiver and press the corresponding button (e.g., TV power)
4. Repeat steps 2-3 for additional button pairs
5. Use the translator in everyday scenarios - when you press the learned button on your primary remote, the translator will send the paired signal to your other device

## Contributing

This project is in early development. Feel free to open issues with suggestions or contribute to the code once it's published.

## License

[License information will be added]
