CC = gcc
CFLAGS = `pkg-config --cflags fftw3 sdl2`
CFLAGS += -Wall -Wextra -g -std=c99
INC = -I../gui -I../visualiser -I../ -I../fft -I. -I..
CFLAGS += $(INC)


wavProcessor.o: wavProcessor.c wavProcessor.h audioData.h ../fft/fftCalculator.h ../settings.h
	$(CC) $(CFLAGS) -c $< -o $@

audioData.o: audioData.c audioData.h ../dataHandler.h wavProcessor.h ../visualiser/visualiser.h ../settings.h
	$(CC) $(CFLAGS) -c $< -o $@

sampler.o: sampler.c sampler.h audioData.h ../settings.h
	$(CC) $(CFLAGS) -c $< -o $@


clean:
	/bin/rm -rf *.o