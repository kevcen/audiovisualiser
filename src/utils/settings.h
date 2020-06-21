#ifndef SETTINGS_H
#define SETTINGS_H

// Visualiser information
#define OPACITY 255
#define OFFWHITE 245
#define FRAMES_TO_SKIP 20

// Colour information
#define NUM_BLEND 5 //how many colours to blend
#define NUM_OF_PALETTES 7
#define MAX_NUM_OF_COLOURS 7
#define MAX_PALETTE_LABEL 30
#define DEFAULT_PALETTE 3
#define WAVE_RIGHT true
#define WAVE true
#define DEFAULT_WAVE_SPEED 3 // lower is faster (int)
#define MAX_WAVE_SPEED 10
#define WAVE_SPEED_THRESHOLD 7
#define WAVE_SPEED_MULTIPLIER 1.5
#define PEAK_SIZE 8
#define PEAK_THRESHOLD 0.72

// File information
#define SUBDIR "../assets/audio/"
#define MAX_FILE_NAME 100

// GUI Window information
#define WIDTH 1600
#define HEIGHT 600
#define H_PADDING 100
#define V_PADDING 100
#define TOP_PADDING -20
#define BAR_WIDTH ((WIDTH - 2 * H_PADDING) / NUM_OF_BINS)
#define MAX_BAR_HEIGHT (HEIGHT - 2 * V_PADDING - TOP_PADDING)

// Audio processing information
#define BIN_MULTIPLIER 10
#define NUM_OF_BINS (NUM_BLEND * BIN_MULTIPLIER)
#define SAMPLES_PER_FRAME 4096
#define LOWEST_FREQ 100
#define HIGHEST_FREQ 12000
#define THICKNESS_OF_BAR 2
#define FFT_OUTPUT_MULTIPLIER 0.25


// Play button information
#define PLAY_WIDTH 720
#define PLAY_HEIGHT 720
#define IMG_WIDTH (WIDTH / 7)
#define IMG_HEIGHT ((PLAY_HEIGHT / PLAY_WIDTH) * IMG_WIDTH)
#define PLAY_X ((WIDTH - IMG_WIDTH) / 2)
#define PLAY_Y ((HEIGHT - IMG_HEIGHT) / 2)
#define WITHIN_PLAY_BOUNDS(x, y) ((x >= PLAY_X && x <= PLAY_X + PLAY_WIDTH) \
&& (y >= PLAY_Y && y <= PLAY_Y + PLAY_WIDTH))


// Visualiser settings
#define OPACITY 255 // window opacity 
#define DECREASE_START_VELOCITY 1.0f // curve smoothing
#define DECREASE_ACCELERATION 2.0f

// Text information
#define FONT_SIZE (HEIGHT / 2)
#define LOADING_TEXT_HEIGHT (HEIGHT / 5)
#define LOADING_TEXT_WIDTH (LOADING_TEXT_HEIGHT * 3)
#define LOADING_TEXT_X ((WIDTH - LOADING_TEXT_WIDTH) / 2)
#define LOADING_TEXT_Y ((HEIGHT - LOADING_TEXT_HEIGHT) / 2)
#define TIME_TEXT_HEIGHT (HEIGHT / 25)
#define TIME_TEXT_WIDTH (TIME_TEXT_HEIGHT * 3)
#define TIME_TEXT_X (H_PADDING)
#define TIME_TEXT_Y (HEIGHT - V_PADDING + 10)
#endif //SETTINGS_H
