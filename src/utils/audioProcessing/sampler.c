#include <math.h>
#include "audioData.h"
#include "sampler.h"

double get8BitSample(Uint8 *buffer, SDL_AudioFormat format) {
    double val = buffer[0];

    if (SDL_AUDIO_ISSIGNED(format)) {
        return val / pow(2, 7);
    }

    return val / pow(2, 8);
}

double get16BitSample(Uint8 *buffer, SDL_AudioFormat format) {
    double val = SDL_AUDIO_ISLITTLEENDIAN(format)
                 ? (((uint16_t) buffer[0]) | ((uint16_t) buffer[1] << 8))
                 : (((uint16_t) buffer[0] << 8) | (uint16_t) buffer[1]);

    if (SDL_AUDIO_ISSIGNED(format))
        return val / pow(2, 15);

    return val / pow(2, 16);
}


double get32BitSample(Uint8 *buffer, SDL_AudioFormat format) {
    double val = SDL_AUDIO_ISLITTLEENDIAN(format)
                 ? ((uint32_t) buffer[0] | ((uint32_t) buffer[1] << 8)
                    | (uint32_t) buffer[2] << 16 | (uint32_t) buffer[3] << 24)
                 : ((uint32_t) buffer[0] << 24 | ((uint32_t) buffer[1] << 16)
                    | (uint32_t) buffer[2] << 8 | (uint32_t) buffer[3]);

    if (SDL_AUDIO_ISSIGNED(format))
        return val / pow(2, 31);

    return val / pow(2, 32);
}

double getSample(Uint8 *buffer, audioData_t *audioData) {
    SDL_AudioFormat format = audioData->audioSpec.format;
    switch (audioData->resolution) {
        case 1:
            return get8BitSample(buffer, format);
        case 2:
            return get16BitSample(buffer, format);
        case 4:
            return get32BitSample(buffer, format);
        default:
            fprintf(stderr, "Resolution of %d bits unsupported",
                    audioData->resolution * 8);
            exit(EXIT_FAILURE);
    }
}
