# Audio Spectrum Visualiser

![Demo](/assets/images/demo.gif)


## Installation
### Download the submodules
Run `git submodule update --init` to download [H. Zeller's LED library](https://github.com/hzeller/rpi-rgb-led-matrix/).

### Dependencies
This project uses SDL2 and its TrueType font library for the UI and FFTW3 for the Fourier transformation.

- SDL2: https://wiki.libsdl.org/Installation
  - SDL TTF: https://www.libsdl.org/projects/SDL_ttf/
- FFTW3: http://www.fftw.org/download.html

### Compile
Run the makefile inside the `src` directory using `make` to compile the source files.

## Usage
### Running 
`./main songName.wav`

To add your own audio wav files, place them under the assets/audio directory.

### Features
```
p - play / pause
right arrow key - fast forward
left arrow key - rewind
up / down arrow keys - change colour theme
b - toggle bar smoothing (default: off)
m - calculate bar height by local maximum (default: on)
v - volume dependent wave speed of colours (default: on)
d - changes colours automatically on high volume (default: off)
x - exit
```

### Running on terminal
Add `terminal` to the end of the run command like this: `./main songName.wav terminal`

### Running on a Raspberry Pi and RGB LED matrix

[![RPIDemo](https://i.imgur.com/gvcS89g.gif)](https://www.youtube.com/watch?v=EV6f_il4w6A)

[Wire up](https://github.com/hzeller/rpi-rgb-led-matrix/blob/master/wiring.md) your Rasberry Pi with the RGB LED matrix and the [power supply](https://learn.adafruit.com/32x16-32x32-rgb-led-matrix/powering) with the LED matrix.

You cannot use your [on-board sound card](https://github.com/hzeller/rpi-rgb-led-matrix/blob/master/README.md#use-minimal-raspbian-distribution) so use an [external USB sound card](https://raspberrypi.stackexchange.com/questions/80072/how-can-i-use-an-external-usb-sound-card-and-set-it-as-default) and set your Raspberry Pi to use it by default.

Then, simply add `pi` to the end of the run command like this: `sudo ./main songName.wav pi` (root permissions will be required to make the H. Zeller's library work properly)
