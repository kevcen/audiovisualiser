CC = gcc
CFLAGS = `pkg-config --cflags fftw3 sdl2`
CFLAGS += -Wall -Wextra -g -std=c99
INC = -I../audioProcessing -I..
CFLAGS += $(INC)


fftCalculator.o: fftCalculator.c fftCalculator.h ../audioProcessing/sampler.h ../audioProcessing/audioData.h ../settings.h
	$(CC) $(CFLAGS) -c $< -o $@ -lm

clean:
	/bin/rm -rf *.o