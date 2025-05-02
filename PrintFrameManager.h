#ifndef PRINTFRAMEMANAGER_H
#define PRINTFRAMEMANAGER_H
#include "ID3v2Frames.h"
#include "ListFramePtr.h"
#include "ID3v2.h"


void PrintFrame_PrintTXTFrames(ListFramePtr *);

void PrintFrame_PrintCOMMFrames(ListFramePtr *);

void PrintFrame_PrintPRIVFrames(ListFramePtr *);

void PrintFrame_PrintAPICFrames(ListFramePtr *);

void PrintFrame_PrintPOPMFrames(ListFramePtr *);

void printTag(ID3TagType *);

void PrintFrame_printFrame(ID3TagType *, char *);

void PrintFrame_listFrames(ID3TagType *);

#endif // PRINTFRAMEMANAGER_H
