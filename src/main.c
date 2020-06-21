#include <stdio.h>
#include "wavProcessor.h"
#include "dataHandler.h"
#include "playButton.h"
#include "audioData.h"
#include "gui.h"
#include "colourPicker.h"
#include "settings.h"
#include "audioText.h"


int main(int argc, char **argv) {
  // Handle arguments of main
  if (argc < 2) {
    fprintf(stderr, "File name not specified\n");
    return EXIT_FAILURE;
  }
  bool terminal = false;
  if (argc >= 3 && !strcmp("terminal", argv[2])) {
    terminal = true;
  }

  // Start with the subdirectory path for the audio paths
  char fullFilename[MAX_FILE_NAME] = SUBDIR;
  // Concatenate with the audio file name
  strcat(fullFilename, argv[1]);


  SDL_AudioDeviceID audioDevice; // Device used to play the audio
  Uint8 *audioBuffer; // The large buffer which will store all audio data

  // Initialise a data structure containing the required data for the callback
  audioData_t audioData;
  gui_t gui;
  colourList_t colourList;
  dataHandler_t dataHandler = {&audioData, &gui, &colourList, terminal};

  prepareColourList(&colourList);
  prepareGUI(&dataHandler, terminal);

  prepareAudioData(&audioDevice, &audioBuffer, &dataHandler, fullFilename);

  // With the data structure, calculate the FFT calculations and store it in a array of values per frame
  processWavFile(&audioData);

  bool start = true;
  bool playing = false;
  if (terminal) {
    printf("Press Enter to Play.\n");
    while(getchar() != '\n');
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



  // fftw cleanup
  fftw_cleanup();

  // SDL cleanup
  SDL_FreeWAV(audioBuffer);
  SDL_CloseAudioDevice(audioDevice);
  if(!terminal) {
    SDL_DestroyRenderer(gui.renderer);
    SDL_DestroyWindow(gui.window);
  }
  SDL_Quit();
  TTF_CloseFont(gui.font);
  TTF_Quit();
  // Free the memory allocated for the colour list
  freeColourList(dataHandler.colourList);
  // Free the members of audio data
  freeAudioData(&audioData);

  printf("Hope you enjoyed.\n");
  return EXIT_SUCCESS;
}
