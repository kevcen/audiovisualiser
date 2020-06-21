#include "colourPicker.h"
#include "audioData.h"
#include <stdbool.h>

#define CALC_WAVE_SPEED(x) ceil(((1 - percentage) * (MAX_WAVE_SPEED - 1)) + 1)
//interpolates between the two boundary colours given
static SDL_Colour *interpolateColour(SDL_Colour a, SDL_Colour b, int index, int bands) {
  SDL_Colour *result = malloc(sizeof(*result));
  result->r = a.r + (b.r - a.r) * ((double) ((index + 1) % bands) / (double) bands);
  result->g = a.g + (b.g - a.g) * ((double) ((index + 1) % bands) / (double) bands);
  result->b = a.b + (b.b - a.b) * ((double) ((index + 1) % bands) / (double) bands);
  return result;
}

/*
//interpolates between 2 colours
static SDL_Colour *getColourOfIthBin(int i, colourList_t *colourList) {
  int binSegments = NUM_OF_BINS / 2
  if(i == NUM_OF_BINS - 1) {
    return interpolateColour(colourList->themes[PALETTE].colours[1], colourList->themes[PALETTE].colours[1], i, binSegments);
  } else if (i < binSegments - 1) {
    return interpolateColour(colourList->themes[PALETTE].colours[1], colourList->themes[PALETTE].colours[0], i, binSegments);
  }
  return interpolateColour(colourList->themes[PALETTE].colours[0], colourList->themes[PALETTE].colours[1], i, binSegments);
}
*/

// interpolates between n colours
static SDL_Colour *getColourOfIthBin(int i, colourList_t *colourList, int n) {
  int binSegments = (NUM_OF_BINS) / n ;
  if(i == NUM_OF_BINS - 1) {
    return interpolateColour(colourList->themes[colourList->currentPalette].colours[n - 1], colourList->themes[colourList->currentPalette].colours[n - 1], i, binSegments);
  } 
  for (int j = 0; j < n; j++) {
    if(i < (j + 1) * binSegments - 1) {
      return interpolateColour(colourList->themes[colourList->currentPalette].colours[(j + n - 1) % n], colourList->themes[colourList->currentPalette].colours[j], i, binSegments);
    }
  } 
  return NULL;
}  

//calculate and setup the colour array
static void populateColours(colourList_t *colourList) {
  for (int i = 0; i < NUM_OF_BINS; i++) colourList->calculatedColours[i] = getColourOfIthBin(i, colourList, NUM_BLEND);
}

//waves to right if true else left
void colourWave(bool right, colourList_t *colourList){
  if (right) {
    SDL_Colour *colour = colourList->calculatedColours[NUM_OF_BINS - 1];
    for(int i = NUM_OF_BINS - 1; i > 0; i--) {
      colourList->calculatedColours[i] = colourList->calculatedColours[i - 1];
    }
    colourList->calculatedColours[0] = colour;
  } else {
    SDL_Colour *colour = colourList->calculatedColours[0];
    for(int i = 0; i < NUM_OF_BINS - 1; i++) {
      colourList->calculatedColours[i] = colourList->calculatedColours[i + 1];
    }
    colourList->calculatedColours[NUM_OF_BINS - 1] = colour;
  }
}

//hold all of the colours and then calculate the correct colours by interpolation
void prepareColourList(colourList_t *colourList) {
  colourList->calculatedColours = (SDL_Colour **) malloc(sizeof(*colourList->calculatedColours) * NUM_OF_BINS);

  SDL_Colour white = {245, 245, 245, 0};
  SDL_Colour black = {8, 8, 8, 0};

  // rainbow
  SDL_Colour r1 = {255, 0, 0, 0};
  SDL_Colour r2 = {255, 127, 0, 0};
  SDL_Colour r3 = {255, 255, 0, 0};
  SDL_Colour r4 = {0, 255, 0, 0};
  SDL_Colour r5 = {0, 0, 255, 0};
  SDL_Colour r6 = {75, 0, 130, 0};
  SDL_Colour r7 = {143, 0, 255, 0};
  palette_t r = {{r1, r2, r3, r4, r5, r6, r7}, 7, white, black, "Rainbow"};

  // lewis
  SDL_Colour a1 = {8, 61, 119, 0};
  SDL_Colour a2 = {235, 235, 211, 0};
  SDL_Colour a3 = {244, 211, 94, 0};
  SDL_Colour a4 = {238, 150, 75, 0};
  SDL_Colour a5 = {249, 87, 56, 0};
  palette_t a = {{a1, a2, a3, a4, a5}, 5, white, black, "Lewis"};


  // kevin
  SDL_Colour bgb = {52, 62, 61, 0};
  SDL_Colour b1 = {174, 220, 192, 0};
  SDL_Colour b2 = {96, 116, 102, 0};
  SDL_Colour b3 = {123, 211, 137, 0};
  SDL_Colour b4 = {24, 180, 130, 0};
  SDL_Colour b5 = {93, 111, 108, 0};
  palette_t b = {{b1, b2, b3, b4, b5}, 5, bgb, white, "Kevin"};

  // lin lin
  SDL_Colour c1 = {8, 76, 97, 0};
  SDL_Colour c2 = {219, 80, 74, 0};
  SDL_Colour c3 = {227, 181, 5, 0};
  SDL_Colour c4 = {79, 109, 122, 0};
  SDL_Colour c5 = {86, 163, 166, 0};
  palette_t c = {{c1, c2, c3, c4, c5}, 5, black, white, "Lin Lin"};

  // gordon
  SDL_Colour d1 = {128, 166, 206, 0};
  SDL_Colour d2 = {149, 184, 209, 0};
  SDL_Colour d3 = {183, 224, 210, 0};
  SDL_Colour d4 = {214, 234, 223, 0};
  SDL_Colour d5 = {234, 196, 213, 0};
  palette_t d = {{d1, d2, d3, d4, d5}, 5, white, black, "Gordon"};

  // yellow
  SDL_Colour e1 = {255, 249, 51, 0};
  SDL_Colour e2 = {255, 249, 51, 0};
  SDL_Colour e3 = {255, 249, 51, 0};
  SDL_Colour e4 = {255, 249, 51, 0};
  SDL_Colour e5 = {255, 249, 51, 0};
  palette_t e = {{e1, e2, e3, e4, e5}, 5, black, white, "BUMBLEBEE"};

  SDL_Colour bgf = {228, 202, 180, 0};
  SDL_Colour f1 = {255, 219, 194, 0};
  SDL_Colour f2 = {245, 203, 163, 0};
  SDL_Colour f3 = {201, 134, 134, 0};
  SDL_Colour f4 = {105, 73, 110, 0};
  SDL_Colour f5 = {185, 157, 190, 0};
  palette_t f = {{f1, f2, f3, f4, f5}, 5, bgf, black, "Hello"};

  colourList->themes[0] = a;
  colourList->themes[1] = b;
  colourList->themes[2] = c;
  colourList->themes[3] = d;
  colourList->themes[4] = e;
  colourList->themes[5] = r;
  colourList->themes[6] = f;

  colourList->currentPalette = DEFAULT_PALETTE;
  colourList->volumeSpeed = true;

  populateColours(colourList);
}

SDL_Colour getBackgroundColour(colourList_t *colourList) {
  return colourList->themes[colourList->currentPalette].backgroundColour;
}

SDL_Colour getForegroundColour(colourList_t *colourList) {
  return colourList->themes[colourList->currentPalette].foregroundColour;
}

SDL_Colour *getColour(int i, colourList_t *colourList) {
  return colourList->calculatedColours[i];
}

void calculateWaveSpeed(dataHandler_t *dataHandler) {
  audioData_t *audioData = dataHandler->audioData;
  double percentage = audioData->maxFrameMagnitude[audioData->framePosition]
                      / audioData->maxMagnitude;


  int *currentWaveSpeed = &dataHandler->colourList->currentWaveSpeed;

  // Stretch speeds
  *currentWaveSpeed = (CALC_WAVE_SPEED(percentage) > WAVE_SPEED_THRESHOLD)
                      ? (CALC_WAVE_SPEED(percentage) * WAVE_SPEED_MULTIPLIER)
                      : (CALC_WAVE_SPEED(percentage) / WAVE_SPEED_MULTIPLIER);

  if (*currentWaveSpeed < 1) {
    *currentWaveSpeed = 1;
  } else if (*currentWaveSpeed > MAX_WAVE_SPEED) {
    *currentWaveSpeed = MAX_WAVE_SPEED;
  }

}

void nextTheme(colourList_t *colourList) {
  colourList->currentPalette = (colourList->currentPalette + 1) % NUM_OF_PALETTES;
  populateColours(colourList);
}

void prevTheme(colourList_t *colourList) {
  colourList->currentPalette = colourList->currentPalette - 1;
  if (colourList->currentPalette < 0)
    colourList->currentPalette = NUM_OF_PALETTES - 1;
  populateColours(colourList);
}

void freeColourList(colourList_t *colourList) {
  for (int i = 0; i < NUM_OF_BINS; i++) free(colourList->calculatedColours[i]);
  free(colourList->calculatedColours);
}


