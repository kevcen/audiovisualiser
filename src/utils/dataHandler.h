#ifndef DATA_HANDLER_H
#define DATA_HANDLER_H
#include <SDL2/SDL.h>
#include <fftw3.h>
#include <stdbool.h>
#include <SDL_ttf.h>
#include "settings.h"


#include "led-matrix-c.h"

typedef enum {
  PI, GUI, TERMINAL
} outputType_t;

typedef struct {
  SDL_Colour colours[MAX_NUM_OF_COLOURS];
  int numberOfColours;
  SDL_Colour backgroundColour;
  SDL_Colour foregroundColour;
  char paletteName[MAX_PALETTE_LABEL];
} palette_t;	

typedef struct {
  palette_t themes[NUM_OF_PALETTES];
  SDL_Colour **calculatedColours;
  int currentPalette; // Current palette / colour scheme
  int currentWaveSpeed; // Current wave speed in the frame
  bool volumeSpeed; // Set makes the wave speed depend on the volume
} colourList_t;

typedef struct {
    // Values per sample in one frame
    fftw_complex *in;
    fftw_complex *out;
    fftw_plan plan;
} fft_t;

typedef struct {
    SDL_AudioSpec audioSpec; // The struct which holds all important information about the audio

    Uint32 audioRemaining; // The number of bytes remaining in the audio
    Uint8 *audioPosition; // Points to the current byte_t reading

    char *filename; // Name of the audio file

    int framePosition; // Which frame on the sample we are currently reading
    int numOfFrames; // Total number of frames in the audio file
  
    int resolution; // Number of bytes per sample which determines the resolution of each sample

    fft_t *fft; // For each channel, stores the data to calculate Fast Fourier Transform to one frame of data
    double **fftResults; // Results of the fft organised in [frame][bin];
    double *bandBuffer; // Stores output of the previous frame
    double *bufferDecrease; // Stores how much the bar heights decrease by

    double *maxBinMagnitude; // Maximum of each bin
    double *maxFrameMagnitude; // Stores maximum magnitude per frame
    double maxMagnitude; // Largest amplitude

    bool peakOn;
    int peakCounter;
    bool showPeak;

    bool buffer;
    bool showTime;
    bool localMax;
    
    struct RGBLedMatrix *matrix;
    struct LedCanvas *offscreen_canvas;
    int matrixWidth, matrixHeight;
    int colourCounter;
} audioData_t;

typedef struct {
  SDL_Window *window;
  SDL_Renderer *renderer;
  TTF_Font *font;
} gui_t;

typedef struct {
  audioData_t *audioData;
  gui_t *gui;
  colourList_t *colourList;
  outputType_t outputType;
} dataHandler_t;

#endif

