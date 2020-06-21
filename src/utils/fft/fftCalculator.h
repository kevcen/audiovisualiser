#ifndef FFTCALCULATOR_H
#define FFTCALCULATOR_H

#include "audioData.h"

int getSamplesPerChannel(int numOfBytes, audioData_t *audioData);

void setInputs(int numOfBytes, Uint8 *buffer, audioData_t *audioData);

void calculateFFT(int numOfBytes, int framePosition,
                  int channel, audioData_t *audioData);

#endif //FFTCALCULATOR_H
