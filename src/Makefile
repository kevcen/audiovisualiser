CC = gcc
CFLAGS = `sdl2-config --cflags`
CFLAGS += -Wall -Wextra -g -std=c99
INC = -Iutils/audioProcessing -Iutils -Iutils/fft -Iutils/visualiser -Iutils/gui
CFLAGS += $(INC)
LDFLAGS =`sdl2-config --libs`
LDFLAGS += -lm -lSDL2_ttf -lfftw3
OBJS = main.o utils/audioProcessing/audioData.o utils/audioProcessing/sampler.o utils/audioProcessing/wavProcessor.o utils/fft/fftCalculator.o utils/visualiser/visualiser.o  utils/gui/gui.o utils/gui/playButton.o utils/gui/colourPicker.o utils/gui/audioText.o

RGB_LIB_DISTRIBUTION=../rpi-rgb-led-matrix
RGB_INCDIR=$(RGB_LIB_DISTRIBUTION)/include
RGB_LIBDIR=$(RGB_LIB_DISTRIBUTION)/lib
RGB_LIBRARY_NAME=rgbmatrix
RGB_LIBRARY=$(RGB_LIBDIR)/lib$(RGB_LIBRARY_NAME).a
LDFLAGS+=-L$(RGB_LIBDIR) -l$(RGB_LIBRARY_NAME) -lrt -lm -lpthread



all: main

$(RGB_LIBRARY): FORCE
	$(MAKE) -C $(RGB_LIBDIR)
	
	
main: $(OBJS) $(RGB_LIBRARY)
	$(CC) $(OBJS) -o $@  $(LDFLAGS) -lstdc++

main.o: main.c utils/dataHandler.h utils/audioProcessing/wavProcessor.h utils/gui/playButton.h utils/audioProcessing/audioData.h utils/gui/colourPicker.h utils/gui/gui.h utils/settings.h
	$(CC) -I$(RGB_INCDIR) $(CFLAGS) -c $< -o $@

utils/audioProcessing/audioData.o: utils/audioProcessing/audioData.c utils/audioProcessing/audioData.h utils/dataHandler.h utils/audioProcessing/wavProcessor.h utils/visualiser/visualiser.h utils/settings.h
	cd utils; make audioProcessing/audioData.o

utils/audioProcessing/sampler.o: utils/audioProcessing/sampler.c utils/audioProcessing/sampler.h  utils/audioProcessing/audioData.h utils/settings.h
	cd utils; make audioProcessing/sampler.o

utils/audioProcessing/wavProcessor.o: utils/audioProcessing/wavProcessor.c utils/audioProcessing/wavProcessor.h utils/audioProcessing/audioData.h utils/fft/fftCalculator.h utils/settings.h
	cd utils; make audioProcessing/wavProcessor.o

utils/fft/fftCalculator.o: utils/fft/fftCalculator.c utils/fft/fftCalculator.h utils/audioProcessing/sampler.h utils/audioProcessing/audioData.h utils/settings.h
	cd utils; make fft/fftCalculator.o

utils/visualiser/visualiser.o: utils/visualiser/visualiser.c utils/visualiser/visualiser.h utils/audioProcessing/audioData.h utils/dataHandler.h utils/gui/colourPicker.h  utils/settings.h
	cd utils; make visualiser/visualiser.o

utils/gui/gui.o: utils/gui/gui.c utils/gui/gui.h utils/gui/playButton.h utils/gui/colourPicker.h utils/dataHandler.h utils/settings.h utils/audioProcessing/audioData.h
	cd utils; make gui/gui.o

utils/gui/playButton.o: utils/gui/playButton.c utils/gui/playButton.h utils/gui/gui.h utils/dataHandler.h utils/audioProcessing/audioData.h utils/settings.h
	cd utils; make gui/playButton.o

utils/gui/colourPicker.o: utils/gui/colourPicker.c utils/gui/colourPicker.h utils/dataHandler.h utils/audioProcessing/audioData.h utils/settings.h
	cd utils; make gui/colourPicker.o

utils/gui/audioText.o: utils/gui/audioText.c utils/gui/audioText.h utils/gui/gui.h utils/dataHandler.h utils/audioProcessing/audioData.h utils/settings.h utils/gui/colourPicker.h
	cd utils; make gui/audioText.o


clean:
	/bin/rm -rf *.o main
	cd utils; make clean

FORCE:
.PHONY: FORCE
