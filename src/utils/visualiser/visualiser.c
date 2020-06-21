#include "visualiser.h"
#include "colourPicker.h"
#include "dataHandler.h"
#include "gui.h"
#include "audioText.h"


static double updateBandBuffer(audioData_t *audioData, int band) {
  double *fftResults = &audioData->fftResults[audioData->framePosition][band];
  if (audioData->buffer) {
    double *bandBuffer = &audioData->bandBuffer[band];
    double *bufferDecrease = &audioData->bufferDecrease[band];

    if (*fftResults >
        *bandBuffer) { // If increase in volume, instantly increase bar height
      // Set band buffer to fftResult
      *bandBuffer = *fftResults;
      *bufferDecrease = DECREASE_START_VELOCITY; // Starting speed
    } else { // If decrease in volume, slowly decrease bar height
      // Decrease band buffer
      *bandBuffer -= *bufferDecrease;
      *bufferDecrease *= DECREASE_ACCELERATION; // Acceleration
    }

    return *bandBuffer;
  }
  return *fftResults;
}


static void displayToTerminal(audioData_t *audioData) {
  // Clear terminal
  system("clear");

  // Output the height for each band
  for (int i = 0; i < NUM_OF_BINS; i++) {
    double magnitude = updateBandBuffer(audioData, i);


    for (int j = 0; j < THICKNESS_OF_BAR; j++) {
      //Rectangle length magnitude
      for (int k = 0; k < magnitude; k++) {
        putchar('-');
      }
      putchar('\n');
    }
  }
  fflush(stdout);
}


static void displayToGUI(dataHandler_t *dataHandler) {
  // Retrieve data members
  audioData_t *audioData = dataHandler->audioData;
  gui_t *gui = dataHandler->gui;
  colourList_t *colourList = dataHandler->colourList;


  // Clear the console to background colour
  SDL_Colour colour = getBackgroundColour(colourList);
  SDL_SetRenderDrawColor(gui->renderer, colour.r, colour.g, colour.b, OPACITY);
  SDL_RenderClear(gui->renderer);

  // Draw time
  if (audioData->showTime) drawTimeText(dataHandler);

  // Changing colour schemes at peaks
  if (audioData->showPeak) {
    double audioPeak = audioData->maxFrameMagnitude[audioData->framePosition]
                       / audioData->maxMagnitude;
    if (!audioData->peakOn && audioData->peakCounter < PEAK_SIZE &&
        audioPeak > PEAK_THRESHOLD) {
      audioData->peakOn = true;
      nextTheme(colourList);
    }
    if (audioData->peakOn) {
      audioData->peakCounter++;
      if (audioData->peakCounter >= PEAK_SIZE) {
        audioData->peakOn = false;
        audioData->peakCounter = 0;
      }
    }
  }

  for (int i = 0; i < NUM_OF_BINS; i++) {
    // Get the magnitude of the current bin
    double magnitude = updateBandBuffer(audioData, i);
    double max = audioData->localMax ? audioData->maxBinMagnitude[i]
                                     : audioData->maxMagnitude;
    // Get the height of each bar
    double barHeight = MAX_BAR_HEIGHT * (magnitude / max);
    if (barHeight < 0) barHeight = 0;

    // Create a rectangle at the correct size and position
    SDL_Rect rect = {BAR_WIDTH * i + H_PADDING,
                     MAX_BAR_HEIGHT - barHeight + V_PADDING + TOP_PADDING,
                     BAR_WIDTH - 1, // -1 makes a gap between each bar
                     barHeight};
    // Set the colour of the rectangle
    SDL_Colour *colour = getColour(i, colourList);
    // printf("Colour %d: (%d, %d, %d)\n", i, colour->r, colour->g, colour->b);
    SDL_SetRenderDrawColor(gui->renderer, colour->r, colour->g, colour->b,
                           OPACITY);

    // Draw the rectangle
    SDL_RenderFillRect(gui->renderer, &rect);
  }

  calculateWaveSpeed(dataHandler);
  int speed = colourList->volumeSpeed ? colourList->currentWaveSpeed : DEFAULT_WAVE_SPEED;
  if(WAVE && !(audioData->framePosition % speed)) {
    colourWave(WAVE_RIGHT, colourList);
  }

  // Refreshes the output of the screen
  SDL_RenderPresent(gui->renderer);
}

void visualiseCallback(void *userData, Uint8 *audioBuffer, int frameSize) {
  // Cast the user data
  dataHandler_t *dataHandler = (dataHandler_t *) userData;
  audioData_t *data = dataHandler->audioData;

  // If there is no more audioRemaining
  if (data->audioRemaining <= 0) return;

  if (dataHandler->terminal) {
    // Display the GUI to terminal
    displayToTerminal(data);
  } else {
    // Visualise the output to gui./m
    displayToGUI(dataHandler);
  }

  // Store the number of bytes to be retrieved in length
  Uint32 length = (Uint32) frameSize;
  if (length > data->audioRemaining) {
    length = data->audioRemaining;
  }

  // Store the 'length' number of bytes from audioPosition in audioBuffer
  SDL_memcpy(audioBuffer, data->audioPosition, length);

  // Increment and decrement respective pointers
  data->audioPosition += length;
  data->audioRemaining -= length;
  data->framePosition++;
}

