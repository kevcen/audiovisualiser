#ifndef SAMPLER_H
#define SAMPLER_H

double getSample(Uint8 *buffer, audioData_t *audioData);

double get8BitSample(Uint8 *buffer, SDL_AudioFormat format);

double get16BitSample(Uint8 *buffer, SDL_AudioFormat format);

double get32BitSample(Uint8 *buffer, SDL_AudioFormat format);

#endif //SAMPLER_H
