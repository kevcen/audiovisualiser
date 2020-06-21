#include "gui.h"
#include "playButton.h"
#include "audioText.h"

void prepareGUI(dataHandler_t *dataHandler, bool terminal){
  gui_t *gui = dataHandler->gui;
  // Initialise SDL library to be used
  if (SDL_Init(terminal ? SDL_INIT_AUDIO : (SDL_INIT_AUDIO | SDL_INIT_VIDEO))) {
    SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
    exit(EXIT_FAILURE);
  }

  if (TTF_Init()) {
    SDL_Log("Unable to initialize TTF: %s", SDL_GetError());
    exit(EXIT_FAILURE);
  }

  // Setup the gui struct
  if(!terminal) {
    gui->window = SDL_CreateWindow(
            "Vibes, only vibes and nothing but the vibes",
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT,
            SDL_WINDOW_BORDERLESS);
    gui->renderer = SDL_CreateRenderer(gui->window, -1,
                                       SDL_RENDERER_ACCELERATED);

    // Load font
    TTF_Font *font = TTF_OpenFont("../assets/fonts/CaviarDreams.ttf", FONT_SIZE);
    if(!font) {
      printf("TTF_OpenFont: %s\n", TTF_GetError());
      exit(EXIT_FAILURE);
    }
    gui->font = font;

    // Draw 'Loading' text
    drawLoadingText(dataHandler);
  }


}


void moveNFrames(int n, dataHandler_t *dataHandler) {
  audioData_t *data = dataHandler->audioData;
  int frameSize = data->audioSpec.size;

  // If we can only skip within our range
  if ((n < 0 || data->audioRemaining > (Uint32) n * frameSize)
          && data->framePosition + n > 0) {
    // Update fields respectively 
    data->audioPosition += n * frameSize;
    data->audioRemaining -= n * frameSize;
    data->framePosition += n;
  }
}

