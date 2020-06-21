#ifndef AUDIODATA_H
#define AUDIODATA_H

#include "dataHandler.h"

void prepareAudioData(SDL_AudioDeviceID *audioDevice, Uint8 **outputBuffer,
                      dataHandler_t *dataHandler, char *filename);

void freeAudioData(audioData_t *audioData);
int getNumOfFrames(audioData_t *audioData, Uint32 length);
double *allocateList(int length);
double **allocateMatrix(int rows, int cols);
void allocateFFT(audioData_t *audioData);
void freeFFT(audioData_t *audioData);
void freeMatrix(double **matrix, int rows);
#endif //AUDIODATA_H
