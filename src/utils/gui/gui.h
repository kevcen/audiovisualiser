#ifndef GUI_H
#define GUI_H
#include "dataHandler.h"
#include "settings.h"
void prepareGUI(dataHandler_t *dataHandler, bool terminal);
void moveNFrames(int n, dataHandler_t *dataHandler);

#endif
