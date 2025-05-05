#ifndef PRINTFRAMEMANAGER_H
#define PRINTFRAMEMANAGER_H
#include "../ID3v2Frames.h"
#include "../ListFramePtr.h"
#include "../ID3v2.h"

void printTag(ID3TagType *);

void PrintFrame_printFrame(ID3TagType *, char *);

void PrintFrame_listFrames(ID3TagType *);


void PrintFrame_APIC(ID3v2APICFrameType, int);
void PrintFrame_TXTF(ID3v2TXTFrameType, int);
void PrintFrame_COMM(ID3v2COMMFrameType, int);
void PrintFrame_PRIV(ID3v2PRIVFrameType, int);
void PrintFrame_MDCI(ID3v2MCDIFrameType, int);
void PrintFrame_POPM(ID3v2POPMFrameType, int);
void PrintFrame_DefaultFrame(ID3v2DefaultFrameType, int);
void PrintFrame_WWWF(ID3v2WWWFrameType, int);
void PrintFrame_WXXX(ID3v2WXXXFrameType, int);
void PrintFrame_TXXX(ID3v2TXXXFrameType ,int);
void PrintFrame_PCNT(ID3v2PCNTFrameType,int);
void PrintFrame_IPLS(ID3v2IPLSFrameType , int );
void PrintFrame_saveAPICImage(ID3v2APICFrameType);
void PrintFrame_USER(ID3v2USERFrameType , int );
void PrintFrame_RVRB(ID3v2RVRBFrameType , int );
#endif // PRINTFRAMEMANAGER_H
