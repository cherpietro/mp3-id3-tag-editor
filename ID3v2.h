#ifndef ID3V2_H
#define ID3V2_H

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "ID3v2Header.h" 
#include "ID3v2Frames.h"
#include "TextFrameList.h"

typedef struct {
    ID3v2HeaderType header;
    TextFrameList textFrameList;
    ID3v2APICFrame *APIC;
} ID3TagType;

void readV2Tag(FILE*, ID3TagType*);

int storeNextFrame(FILE*, ID3TagType*);

void initID3v2Tag(ID3TagType *);

void freeID3v2Tag(ID3TagType *);

#endif // ID3V2_H
