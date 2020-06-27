CC = gcc
CFLAGS = `pkg-config --cflags fftw3 sdl2`
CFLAGS += -Wall -Wextra -g -std=c99
INC = -I../audioProcessing -I.. -I../gui
CFLAGS += $(INC)

RGB_LIB_DISTRIBUTION=../../../rpi-rgb-led-matrix
RGB_INCDIR=$(RGB_LIB_DISTRIBUTION)/include
visualiser.o: visualiser.c visualiser.h ../audioProcessing/audioData.h ../dataHandler.h ../gui/colourPicker.h  ../settings.h ../gui/audioText.h
	$(CC) -I$(RGB_INCDIR) $(CFLAGS) -c $< -o $@

clean:
	/bin/rm -rf *.o
