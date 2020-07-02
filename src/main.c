#include <stdio.h>
#include "wavProcessor.h"
#include "dataHandler.h"
#include "playButton.h"
#include "audioData.h"
#include "gui.h"
#include "colourPicker.h"
#include "settings.h"
#include "audioText.h"

#include<signal.h>
#include<unistd.h>

dataHandler_t dataHandler;
SDL_AudioDeviceID audioDevice; // Device used to play the audio
Uint8 *audioBuffer; // The large buffer which will store all audio data

void cleanup(void) {
  if(dataHandler.outputType == PI)
    led_matrix_delete(dataHandler.audioData->matrix);

  // fftw cleanup
  fftw_cleanup();

  // SDL cleanup
  SDL_FreeWAV(audioBuffer);
  SDL_CloseAudioDevice(audioDevice);
  if(dataHandler.outputType == GUI) {
    SDL_DestroyRenderer(dataHandler.gui->renderer);
    SDL_DestroyWindow(dataHandler.gui->window);
    TTF_CloseFont(dataHandler.gui->font);
  } 
  SDL_Quit();
  TTF_Quit();
  // Free the memory allocated for the colour list
  freeColourList(dataHandler.colourList);
  // Free the members of audio data
  freeAudioData(dataHandler.audioData);
}


void sigHandler(int sig){
  if (sig == SIGINT) printf("Exit detected, cleaning up program.");
  cleanup();
  printf("Hope you enjoyed.\n");
}


int main(int argc, char **argv) {
  signal(SIGINT, sigHandler); // Register signal handler
  
  
  // Handle arguments of main
  if (argc < 2) {
    fprintf(stderr, "File name not specified\n");
    return EXIT_FAILURE;
  }
  outputType_t outputType = GUI;
  if (argc >= 3) {
    if(!strcmp("terminal", argv[2])) {
      outputType = TERMINAL;
    } else if(!strcmp("pi", argv[2])) {
      outputType = PI;
    }
  }

  // Start with the subdirectory path for the audio paths
  char fullFilename[MAX_FILE_NAME] = SUBDIR;
  // Concatenate with the audio file name
  strcat(fullFilename, argv[1]);



  // Initialise a data structure containing the required data for the callback
  audioData_t audioData;
  gui_t gui;
  colourList_t colourList;
  //dataHandler = {&audioData, &gui, &colourList, terminal};
  dataHandler.audioData = &audioData;
  dataHandler.gui = &gui;
  dataHandler.colourList = &colourList;
  dataHandler.outputType = outputType;

  prepareColourList(&colourList);
  prepareGUI(&dataHandler);

  prepareAudioData(&audioDevice, &audioBuffer, &dataHandler, fullFilename,
              &argc, &argv);

  // With the data structure, calculate the FFT calculations and store it in a array of values per frame
  processWavFile(&audioData);

  bool start = true;
  bool playing = false;
  if (outputType != GUI) {
    if (outputType == TERMINAL) {
      printf("Press Enter to Play.\n");
      while(getchar() != '\n');
    }
    // Play the audio and wait for the callback to work
    SDL_PauseAudioDevice(audioDevice, 0);
    // Block the program whilst there is still audio remaining
    while (audioData.audioRemaining > 0);
  } else {

    audioData.buffer = false; //Looks better without buffer
    // Draw play button
    drawPlayButton(&dataHandler);
    // Whilst running, look for events
    SDL_Event event;
    bool running = true;
    while (audioData.audioRemaining > 0 && running) {
      while (SDL_PollEvent(&event)) {
        switch (event.type) {
          case SDL_QUIT:
            running = false; // Quit main loop if user quits
            break;
          case SDL_MOUSEBUTTONDOWN:
            if (WITHIN_PLAY_BOUNDS(event.button.x, event.button.y) && start) {
              // Play the audio and wait for the callback to work
              SDL_PauseAudioDevice(audioDevice, 0);
              playing = true;
              start = false;
            }
            break;
          case SDL_KEYDOWN:
            if(event.type == SDL_KEYDOWN)
            {
              switch(event.key.keysym.sym)
              {
                case SDLK_x: // Also quit if x is pressed
                  audioData.audioRemaining = 0; // Incase the callback is called again
                  running = false;
                  break;
                case SDLK_p: // Play/pause if p is pressed
                  if (start) start = false;
                  SDL_PauseAudioDevice(audioDevice, playing);
                  playing=!playing;
                  break;
                case SDLK_v: // Use with/without volume dependent speed 'v'
                  colourList.volumeSpeed = !colourList.volumeSpeed;
                  break;
                case SDLK_b: // Use with/without buffer if b is pressed
                  audioData.buffer = !audioData.buffer;
                  break;
                case SDLK_m: // Use with/without local max if m pressed
                  audioData.localMax = !audioData.localMax;
                  break;
                case SDLK_t: // Show/not show time with 't'
                  audioData.showTime = !audioData.showTime;
                  break;
                case SDLK_d: // Change colour schemes at peaks
                  audioData.showPeak = !audioData.showPeak;
                  break;
                case SDLK_UP:
                  nextTheme(&colourList);
                  break;
                case SDLK_DOWN:
                  prevTheme(&colourList);
                  break;
                case SDLK_RIGHT:
                  moveNFrames(FRAMES_TO_SKIP, &dataHandler);
                  break;
                case SDLK_LEFT:
                  moveNFrames(-FRAMES_TO_SKIP, &dataHandler);
                  break;
              }
            }
        }
      }
    }
  }


  cleanup();

  printf("Hope you enjoyed.\n");
  return EXIT_SUCCESS;
}
