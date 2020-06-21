
#include <math.h>
#include "fftCalculator.h"
#include "sampler.h"

#ifndef M_PI
  #define M_PI 3.14159265358979323846
#endif

#define HANN(n, N, x) ((0.5 * (1 - cos(2 * M_PI * n / N))) * x)

int getSamplesPerChannel(int numOfBytes, audioData_t *audioData) {
  // numOfBytes can differ to audioSpec.size so must calculate it
  int resolution = audioData->resolution;
  int channels = audioData->audioSpec.channels;
  return numOfBytes / (channels * resolution);
}

void setInputs(int numOfBytes, Uint8 *buffer, audioData_t *audioData) {
  int samplesPerChannel = getSamplesPerChannel(numOfBytes, audioData);

  // Set up the plan per channel
  for (int i = 0; i < audioData->audioSpec.channels; i++) {
    // Use one-dimensional discrete fourier transform
    audioData->fft[i].plan = fftw_plan_dft_1d(samplesPerChannel,
                                              audioData->fft[i].in,
                                              audioData->fft[i].out,
                                              FFTW_FORWARD,
                                              FFTW_MEASURE); // Measure for accuracy
  }

  Uint8 *position = buffer; // Point to the start of the buffer
  // For each sample per channel
  for (int i = 0; i < samplesPerChannel; i++) {
    //For each channel
    for (int j = 0; j < audioData->audioSpec.channels; j++) {
      // The real part is the sample data at that position
      audioData->fft[j].in[i][0] = HANN(i, samplesPerChannel, getSample(position, audioData));
      // The imag part is set to 0
      audioData->fft[j].in[i][1] = 0.0f;
      position += audioData->resolution; // Point to address of next sample
    }
  }
}

void calculateFFT(int numOfBytes, int framePosition,
                  int channel, audioData_t *audioData) {

  // Execute FFT, which sets fft[channel].out values
  fftw_execute(audioData->fft[channel].plan);

  // Set frequency intervals of the output
//  double nyquistFreq = audioData->audioSpec.freq / 2;
  double freqBin[NUM_OF_BINS + 1];
  /* Nyquist Freq / (Base ^ Num_Bins) = Lowest Freq
   * Base = (Nyquist / Lowest Freq) ^ (1 / Num_Bins)
   */
  double base = pow(HIGHEST_FREQ / LOWEST_FREQ, 1.0 / NUM_OF_BINS);

  for (int i = 0; i < NUM_OF_BINS + 1; i++) {
    freqBin[i] = HIGHEST_FREQ / (pow(base, NUM_OF_BINS - i));
  }

  // avArray stores average
  double avArray[NUM_OF_BINS];
  double avCnt[NUM_OF_BINS];
  for (int i = 0; i < NUM_OF_BINS;i ++) {
    avArray[i] = 0;
    avCnt[i] = 0;
  }


  int samplesPerChannel = getSamplesPerChannel(numOfBytes, audioData);
  // Divide by two for to only go up to nyquist frequency
  for (int i = 0; i < samplesPerChannel / 2; i++) {
    // Calculate magnitude at current frequency
    double re = audioData->fft[channel].out[i][0];
    double im = audioData->fft[channel].out[i][1];
    double magnitude = sqrt(re * re + im * im);
    // Calculate current frequency
    double freq = i * (double) audioData->audioSpec.freq / samplesPerChannel;
    for (int j = 0; j < NUM_OF_BINS; j++) {
      // Update max array for the correct bin if value is larger than current max

      if (freq >= freqBin[j] && freq < freqBin[j + 1]) {
        avArray[j] += magnitude;
        avCnt[j]++;
      }
    }
  }

  // Update value of fftResults
  for (int i = 0; i < NUM_OF_BINS; i++) {
    double average = avArray[i] / avCnt[i];
    //If there is no value within this band, find the average of the surrounding bands
    if(average <= 0) {
      double av1 = avArray[i + 1] / avCnt[i + 1];
      double av2 = avArray[i - 1] / avCnt[i - 1];
      if (i == 0) average = av1;
      else if (i == NUM_OF_BINS - 1) average = av2;
      else average = (av1 + av2) / 2;
    }



    // Divide by number of channels and increment to get an average of all channels
    audioData->fftResults[framePosition][i] +=
              average / audioData->audioSpec.channels * FFT_OUTPUT_MULTIPLIER;
  }


  // Set max magnitude
  for (int i = 0; i < NUM_OF_BINS; i++) {
    double currentValue = audioData->fftResults[framePosition][i];
    if (audioData->maxBinMagnitude[i] < currentValue) {
      audioData->maxBinMagnitude[i] = currentValue;
    }
    if (audioData->maxFrameMagnitude[framePosition] < currentValue) {
      audioData->maxFrameMagnitude[framePosition] = currentValue;
    }
    if (audioData->maxMagnitude < currentValue) {
      audioData->maxMagnitude = currentValue;
    }
  }

  // Destroy the plan
  fftw_destroy_plan(audioData->fft[channel].plan);
}
