#include "fftCalculator.h"
#include "wavProcessor.h"

static void skipHeader(audioData_t *audioData, FILE *file) {
  // Get total file size: header info data + audio data
  fseek(file, 0, SEEK_END);
  int fileSize = ftell(file);

  // Seek past header information
  fseek(file, fileSize - audioData->audioRemaining, SEEK_SET);
}

void processWavFile(audioData_t *audioData) {
  // Open audio file for reading
  FILE *file = fopen(audioData->filename, "r");
  if(!file) {
    perror("Error opening file");
    exit(EXIT_FAILURE);
  }

  // Skips the header information of the wav file
  skipHeader(audioData, file);

  // Get size of one frame
  int sizeOfFrame = audioData->audioSpec.size;

  // Buffer to store one frame of data
  Uint8 *buffer = (Uint8 *) malloc(sizeOfFrame * sizeof(Uint8));

  size_t numOfBytes; // Stores the number of bytes read
  int currentFrame = 0; // Stores the current frame position
  while((numOfBytes = fread(buffer, 1, sizeOfFrame, file)) > 0) {
    // Set the fft.in and plan
    setInputs(numOfBytes, buffer, audioData);

    for(int i = 0; i < audioData->audioSpec.channels; i++) {
      // Execute the plan and store it in fftResults of the current frame
      calculateFFT(numOfBytes, currentFrame, i, audioData);
    }

    // Increment to next frame position
    currentFrame++;
  }

  // Close the file
  fclose(file);

  // Memory management
  free(buffer);
}




