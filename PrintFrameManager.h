#ifndef PRINTFRAMEMANAGER_H
#define PRINTFRAMEMANAGER_H
#include "ID3v2Frames.h"
#include "ListFramePtr.h"
#include "ID3v2.h"

void printTag(ID3TagType *);

void PrintFrame_printFrame(ID3TagType *, char *);

void PrintFrame_listFrames(ID3TagType *);


void PrintFrame_APIC(ID3v2APICFrameType);
void PrintFrame_TXTF(ID3v2TXTFrameType);
void PrintFrame_COMM(ID3v2COMMFrameType);
void PrintFrame_PRIV(ID3v2PRIVFrameType);
void PrintFrame_MDCI(ID3v2MCDIFrameType);
void PrintFrame_POPM(ID3v2POPMFrameType);
void PrintFrame_DefaultFrame(ID3v2DefaultFrameType);
void PrintFrame_WWWF(ID3v2WWWFrameType);
void PrintFrame_WXXX(ID3v2WXXXFrameType);

void PrintFrame_saveAPICImage(ID3v2APICFrameType);

#endif // PRINTFRAMEMANAGER_H
