#include "playButton.h"
#include "colourPicker.h"

void drawPlayButton(dataHandler_t *dataHandler) {
  gui_t *gui = dataHandler->gui;
//  colourList_t *colourList = dataHandler->colourList;

  //Clear background
  SDL_SetRenderDrawColor(gui->renderer, OFFWHITE, OFFWHITE, OFFWHITE, OPACITY);

  SDL_RenderClear(gui->renderer);

  // Set dimensions
  SDL_Rect src = {0, 0, PLAY_WIDTH, PLAY_HEIGHT};
  SDL_Rect dest = {PLAY_X, PLAY_Y, IMG_WIDTH, IMG_HEIGHT};

  // Image load
  SDL_Surface *image = SDL_LoadBMP("../assets/images/playButton.bmp");
  if(!image) {
    fprintf(stderr, "Loading image error: %s\n", SDL_GetError());
    exit(EXIT_FAILURE);
  }

  SDL_Texture *texture = SDL_CreateTextureFromSurface(gui->renderer, image);
  if(!image) {
    fprintf(stderr, "Creating surface error : %s\n", SDL_GetError());
    exit(EXIT_FAILURE);
  }

  // Copy image onto renderer
  SDL_RenderCopy(gui->renderer, texture, &src, &dest);

  SDL_FreeSurface(image); // Free surface
  SDL_DestroyTexture(texture); // Free texture

  // Update screen
  SDL_RenderPresent(gui->renderer);
}