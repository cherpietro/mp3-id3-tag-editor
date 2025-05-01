#ifndef PRINTFRAMEMANAGER_H
#define PRINTFRAMEMANAGER_H
#include "ID3v2Frames.h"
#include "ListFramePtr.h"

void PrintFrame_PrintTXTFrames(ListFramePtr *);

void PrintFrame_PrintCOMMFrames(ListFramePtr *);

void PrintFrame_PrintPRIVFrames(ListFramePtr *);

void PrintFrame_PrintAPICFrames(ListFramePtr *);

void PrintFrame_PrintPOPMFrames(ListFramePtr *);

#endif // PRINTFRAMEMANAGER_H
