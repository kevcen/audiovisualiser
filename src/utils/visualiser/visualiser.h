#ifndef VISUALISER_H
#define VISUALISER_H

#include "audioData.h"
#include <SDL2/SDL.h>

void visualiseCallback(void *userData, Uint8* audioBuffer, int frameSize);

#endif //VISUALISER_H
