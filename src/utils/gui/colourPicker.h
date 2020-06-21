#ifndef COLOUR_PICKER_H
#define COLOUR_PICKER_H
#include <SDL2/SDL.h>
#include "dataHandler.h"
#include "settings.h"

void prepareColourList(colourList_t *colourList);
void colourWave(bool right, colourList_t *colourList);
void freeColourList(colourList_t *colourList);
SDL_Colour *getColour(int i, colourList_t *colourList);
SDL_Colour getBackgroundColour( colourList_t *colourList);
SDL_Colour getForegroundColour( colourList_t *colourList);
void nextTheme(colourList_t *colourList);
void prevTheme(colourList_t *colourList);
void calculateWaveSpeed(dataHandler_t *dataHandler);

#endif
