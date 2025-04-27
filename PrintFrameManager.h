#ifndef PRINTFRAMEMANAGER_H
#define PRINTFRAMEMANAGER_H
#include "ID3v2Frames.h"
#include "TXTFrameList.h"
#include "COMMFrameList.h"
#include "PRIVFrameList.h"

void PrintFrame_PrintTXTFrames(TXTFrameList *);

void PrintFrame_PrintCOMMFrames(COMMFrameList *);

void PrintFrame_PrintPRIVFrames(PRIVFrameList *);
#endif // PRINTFRAMEMANAGER_H
