# Audio Spectrum Visualiser

![Demo](/assets/images/demo.gif)



## Installation
### Clone
Clone the repository to your local machine `https://github.com/kevcen/audiovisualiser`

### Dependencies
This project uses SDL2 and its TrueType font library for the UI and FFTW3 for the Fourier transformation.

- SDL2: https://wiki.libsdl.org/Installation
  - SDL TTF: https://www.libsdl.org/projects/SDL_ttf/
- FFTW3: http://www.fftw.org/download.html

### Compile
Run the makefile inside the src directory using `make` to compile the source files.

## Usage
## Running 
`./main songName.wav`

To add your own audio wav files, place them under the assets/audio directory.

## Features
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
