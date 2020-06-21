#include "audioText.h"
#include "colourPicker.h"

void drawLoadingText(dataHandler_t *dataHandler) {
  gui_t *gui = dataHandler->gui;
  colourList_t *colourList = dataHandler->colourList;


  // Set background colour
  SDL_Colour bgColour = getBackgroundColour(colourList);
  SDL_SetRenderDrawColor(gui->renderer, bgColour.r, bgColour.g, bgColour.b, OPACITY);
  SDL_RenderClear(gui->renderer);

  SDL_Colour fgColour = getForegroundColour(colourList);
  SDL_Surface *surfaceMessage = TTF_RenderText_Solid(gui->font, "Loading",
                                                     fgColour);

  if(!surfaceMessage) {
    perror("Surface not loaded");
    exit(EXIT_FAILURE);
  }
  SDL_Texture* message = SDL_CreateTextureFromSurface(gui->renderer, surfaceMessage);

  if(!message) {
    perror("message not loaded");
    exit(EXIT_FAILURE);
  }
  SDL_Rect dest = {LOADING_TEXT_X, LOADING_TEXT_Y, LOADING_TEXT_WIDTH,
                   LOADING_TEXT_HEIGHT};

  SDL_RenderCopy(gui->renderer, message, NULL, &dest);

  SDL_FreeSurface(surfaceMessage);
  SDL_DestroyTexture(message);

  // Update screen
  SDL_RenderPresent(gui->renderer);
}

#define SECONDS_PER_MIN 60

static int calculateTime(dataHandler_t *dataHandler) {
  audioData_t *audioData = dataHandler->audioData;
  int bytesPerSample = audioData->resolution * audioData->audioSpec.channels;
  int samplesRemaining = audioData->audioRemaining / bytesPerSample;
  int totalSamples = audioData->numOfFrames * audioData->audioSpec.samples;
  int lastSampleSize = samplesRemaining % audioData->audioSpec.samples;
  totalSamples -= (audioData->audioSpec.samples - lastSampleSize);
  return (totalSamples - samplesRemaining) / audioData->audioSpec.freq;
}

void drawTimeText(dataHandler_t *dataHandler) {
  gui_t *gui = dataHandler->gui;
  colourList_t *colourList = dataHandler->colourList;

  int totalSeconds = calculateTime(dataHandler);
  int seconds = totalSeconds % SECONDS_PER_MIN;
  int minutes = totalSeconds / SECONDS_PER_MIN;

  char time[6];
  sprintf(time, "%02d:%02d", minutes, seconds);

  SDL_Colour fgColour = getForegroundColour(colourList);
  SDL_Surface *surface = TTF_RenderText_Solid(gui->font, time, fgColour);

  if (!surface) {
    perror("Surface not loaded");
    exit(EXIT_FAILURE);
  }
  SDL_Texture *timeText = SDL_CreateTextureFromSurface(gui->renderer, surface);

  if (!timeText) {
    perror("message not loaded");
    exit(EXIT_FAILURE);
  }
  SDL_Rect dest = {TIME_TEXT_X, TIME_TEXT_Y, TIME_TEXT_WIDTH, TIME_TEXT_HEIGHT};

  SDL_RenderCopy(gui->renderer, timeText, NULL, &dest);

  SDL_FreeSurface(surface);
  SDL_DestroyTexture(timeText);
}