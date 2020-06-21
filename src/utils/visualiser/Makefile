CC = gcc
CFLAGS = `pkg-config --cflags fftw3 sdl2`
CFLAGS += -Wall -Wextra -g -std=c99
INC = -I../audioProcessing -I.. -I../gui
CFLAGS += $(INC)

visualiser.o: visualiser.c visualiser.h ../audioProcessing/audioData.h ../dataHandler.h ../gui/colourPicker.h  ../settings.h ../gui/audioText.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	/bin/rm -rf *.o