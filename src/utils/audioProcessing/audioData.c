#include "wavProcessor.h"
#include "visualiser.h"
#include "dataHandler.h"
#include <math.h>


int getNumOfFrames(audioData_t *audioData, Uint32 length) {
  return ceil((double) length / (audioData->audioSpec.channels
      * audioData->audioSpec.samples * audioData->resolution));
}

void allocateFFT(audioData_t *audioData) {
  int samplesPerFrame = audioData->audioSpec.samples;
  int channels = audioData->audioSpec.channels;

  // Allocate array of fft members for each channel
  audioData->fft = (fft *) malloc(channels * sizeof(fft));
  if (!audioData->fft) {
    perror("Memory allocation to fast fourier transform failed");
    exit(EXIT_FAILURE);
  }
  for(int i = 0; i < channels; i++) {
    // Allocate the fft.in for each channel
    audioData->fft[i].in = (fftw_complex *) fftw_malloc(sizeof(fftw_complex) * samplesPerFrame);
    if (!audioData->fft[i].in) {
      perror("Memory allocation to fft in failed");
      exit(EXIT_FAILURE);
    }
    // Allocate the fft.out for each channel
    audioData->fft[i].out = (fftw_complex *) fftw_malloc(sizeof(fftw_complex) * samplesPerFrame);
    if (!audioData->fft[i].out) {
      perror("Memory allocation to fft out failed");
      exit(EXIT_FAILURE);
    }
  } 
}

double **allocateMatrix(int rows, int cols) {
  // Allocate spine
  double **matrix = (double **) malloc(rows * sizeof(double *));
  if(!matrix) {
    perror("Cannot allocate memory to matrix spine");
    exit(EXIT_FAILURE);
  }

  // Allocate each row per frame
  for(int i = 0; i < rows; i++) {
    // Number of elements for each row is the number of bins
    matrix[i] = (double *) calloc(cols, sizeof(double));
    if(!matrix[i]) {
      perror("Cannot allocate memory to matrix row");
      exit(EXIT_FAILURE);
    }
  }

  return matrix;
}

double *allocateList(int length) {
  double *list = (double *) calloc(length, sizeof(double));
  if(!list) {
    perror("Cannot allocate memory to list");
    exit(EXIT_FAILURE);
  }
  return list;
}

void freeFFT(audioData_t *audioData) {
  for(int i = 0; i < audioData->audioSpec.channels; i++) {
    // Free the fft.in and fft.out for each channel
    fftw_free(audioData->fft[i].in);
    fftw_free(audioData->fft[i].out);
  }
  // Free the fft array
  free(audioData->fft);
}

void freeMatrix(double **matrix, int rows) {
  for(int i = 0; i < rows; i++) {
    // Free each row
    free(matrix[i]);
  }
  // Free the spine
  free(matrix);
}

void freeAudioData(audioData_t *audioData) {
  // Free FFT data
  freeFFT(audioData);
  // Free results matrix
  freeMatrix(audioData->fftResults, audioData->numOfFrames);

  // Free band buffer lists
  free(audioData->bandBuffer);
  free(audioData->bufferDecrease);

  // Free max magnitudes
  free(audioData->maxFrameMagnitude);
  free(audioData->maxBinMagnitude);
}
void initialiseLEDs(audioData_t *audioData, int *argc, char ***argv) {
  struct RGBLedMatrixOptions options;
  
  memset(&options, 0, sizeof(options));
  
  options.rows = 32;
  options.chain_length = 1;
  
  audioData->matrix = led_matrix_create_from_options(&options, argc, argv);
  
  if (audioData->matrix == NULL) {
    fprintf(stderr, "Matrix could not be created");
    exit(EXIT_FAILURE);
  }
  
  
  audioData->offscreen_canvas = led_matrix_create_offscreen_canvas(audioData->matrix);

  led_canvas_get_size(audioData->offscreen_canvas,
      &(audioData->matrixWidth), &(audioData->matrixHeight));
      
  
  fprintf(stderr, "Size: %dx%d. Hardware gpio mapping: %s\n",
          audioData->matrixWidth, audioData->matrixHeight, options.hardware_mapping);
          
        
}
void prepareAudioData(SDL_AudioDeviceID *audioDevice, Uint8 **outputBuffer,
                      dataHandler_t *dataHandler, char *filename,
                      int *argc, char ***argv) {


  audioData_t *audioData = dataHandler->audioData;

  SDL_AudioSpec desired, obtained;
  Uint8 *audioBuffer; // The large buffer which will store all audio data
  Uint32 audioLength; // Length of the audio in the file in bytes

  // Load the wav file
  if (!SDL_LoadWAV(filename, &desired, &audioBuffer, &audioLength)) {
    SDL_Log("Cannot load %s : %s", filename, SDL_GetError());
    exit(EXIT_FAILURE);
  }

  // Set desired properties of the audio spec
  desired.callback = visualiseCallback;
  desired.userdata = dataHandler;
  // Samples per frame, inversely proportional to frames per second
  desired.samples = SAMPLES_PER_FRAME;

  // Open audio device to be later played
  *audioDevice = SDL_OpenAudioDevice(NULL, 0, &desired, &obtained,
          SDL_AUDIO_ALLOW_ANY_CHANGE);
  if(!*audioDevice) {
    SDL_Log("Failed to open audio: %s", SDL_GetError());
    exit(EXIT_FAILURE);
  }

  // Set members of audioData to retrieved information
  audioData->audioSpec = obtained;

  // The audio remaining at the start is the total length of of the audio
  audioData->audioRemaining = audioLength;
  // Points to the first element in the audio buffer
  audioData->audioPosition = audioBuffer;

  audioData->resolution = SDL_AUDIO_BITSIZE(obtained.format) / 8;

  audioData->numOfFrames = getNumOfFrames(audioData, audioLength);
  audioData->framePosition = 0;

  audioData->filename = filename;
  // Allocate memory for audioData->fft
  allocateFFT(audioData);
  // Allocate memory for audioData->fftResults
  audioData->fftResults = allocateMatrix(audioData->numOfFrames, NUM_OF_BINS);

  // Allocate memory for the band buffers
  audioData->bandBuffer = allocateList(NUM_OF_BINS);
  audioData->bufferDecrease = allocateList(NUM_OF_BINS);

  // Allocate memory for max magnitudes per frame
  audioData->maxFrameMagnitude = allocateList(audioData->numOfFrames);

  // Allocate memory for max magnitudes per bin
  audioData->maxBinMagnitude = allocateList(NUM_OF_BINS);

  // Max magnitude of audio
  audioData->maxMagnitude = 0.0;

  // Whether to use the band buffers
  audioData->buffer = true;

  // Whether to display time
  audioData->showTime = false;

  // Use local max or not
  audioData->localMax = false;

  audioData->peakOn = false;
  audioData->peakCounter = 0;
  audioData->showPeak = false;

  initialiseLEDs(audioData, argc, argv);
  // Set the output buffer to point to the audioBuffer to be later free'd
  *outputBuffer = audioBuffer;
}
