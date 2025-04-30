#ifndef PRINTFRAMEMANAGER_H
#define PRINTFRAMEMANAGER_H
#include "ID3v2Frames.h"
#include "ListFramePtr.h"
// // #include "TXTFrameList.h"
// #include "COMMFrameList.h"
// #include "PRIVFrameList.h"

void PrintFrame_PrintTXTFrames(ListFramePtr *);

void PrintFrame_PrintCOMMFrames(ListFramePtr *);

void PrintFrame_PrintPRIVFrames(ListFramePtr *);
#endif // PRINTFRAMEMANAGER_H
