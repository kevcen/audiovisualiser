audioProcessing/audioData.o: audioProcessing/audioData.c audioProcessing/audioData.h dataHandler.h audioProcessing/wavProcessor.h visualiser/visualiser.h settings.h
	cd audioProcessing; make audioData.o

audioProcessing/sampler.o: audioProcessing/sampler.c audioProcessing/sampler.h audioProcessing/audioData.h settings.h
	cd audioProcessing; make sampler.o

audioProcessing/wavProcessor.o: audioProcessing/wavProcessor.c audioProcessing/wavProcessor.h audioProcessing/audioData.h fft/fftCalculator.h settings.h
	cd audioProcessing; make wavProcessor.o

fft/fftCalculator.o: fft/fftCalculator.c fft/fftCalculator.h audioProcessing/sampler.h audioProcessing/audioData.h settings.h
	cd fft; make fftCalculator.o

visualiser/visualiser.o: visualiser/visualiser.c visualiser/visualiser.h audioProcessing/audioData.h dataHandler.h gui/colourPicker.h  settings.h
	cd visualiser; make visualiser.o

gui/gui.o: gui/gui.c gui/gui.h gui/playButton.h gui/colourPicker.h dataHandler.h settings.h audioProcessing/audioData.h
	cd gui; make gui.o

gui/playButton.o: gui/playButton.c gui/playButton.h dataHandler.h gui/gui.h audioProcessing/audioData.h settings.h
	cd gui; make playButton.o

gui/audioText.o: gui/audioText.c gui/audioText.h dataHandler.h gui/gui.h audioProcessing/audioData.h settings.h gui/colourPicker.h
	cd gui; make audioText.o


gui/colourPicker.o: gui/colourPicker.c gui/colourPicker.h dataHandler.h audioProcessing/audioData.h settings.h
	cd gui; make colourPicker.o

clean:
	cd visualiser; make clean
	cd gui; make clean
	cd fft; make clean
	cd audioProcessing; make clean
