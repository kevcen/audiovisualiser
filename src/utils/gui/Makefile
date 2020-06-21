CC = gcc
CFLAGS = `pkg-config --cflags fftw3 sdl2`
CFLAGS += -Wall -Wextra -g -std=c99
INC = -I../ -I../audioProcessing
CFLAGS += $(INC)


playButton.o: playButton.c playButton.h ../dataHandler.h gui.h ../audioProcessing/audioData.h ../settings.h colourPicker.h
	$(CC) $(CFLAGS) -c $< -o $@

gui.o: gui.c gui.h playButton.h colourPicker.h ../dataHandler.h ../settings.h ../audioProcessing/audioData.h audioText.h
	$(CC) $(CFLAGS) -c $< -o $@

colourPicker.o: colourPicker.c colourPicker.h ../dataHandler.h ../audioProcessing/audioData.h ../settings.h
	$(CC) $(CFLAGS) -c $< -o $@

audioText.o: audioText.c audioText.h ../dataHandler.h gui.h ../audioProcessing/audioData.h ../settings.h colourPicker.h
	$(CC) $(CFLAGS) -c $< -o $@



clean:
	/bin/rm -rf *.o