CC = gcc
CFLAGS = `pkg-config --cflags fftw3 sdl2`
CFLAGS += -Wall -Wextra -g -std=c99
INC = -I../audioProcessing -I..
CFLAGS += $(INC)

RGB_LIB_DISTRIBUTION=../../../rpi-rgb-led-matrix
RGB_INCDIR=$(RGB_LIB_DISTRIBUTION)/include

fftCalculator.o: fftCalculator.c fftCalculator.h ../audioProcessing/sampler.h ../audioProcessing/audioData.h ../settings.h
	$(CC) -I$(RGB_INCDIR) $(CFLAGS) -c $< -o $@ -lm

clean:
	/bin/rm -rf *.o
