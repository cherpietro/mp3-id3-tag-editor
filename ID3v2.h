#ifndef ID3V2_H
#define ID3V2_H

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "ID3v2Header.h" 
#include "ID3v2Frames.h"
#include "TextFrameList.h"
#include "COMMFrameList.h"

typedef struct {
    ID3v2HeaderType header;
    TextFrameList textFrameList;
    COMMFrameList COMMFrameList;
    ID3v2APICFrame *APIC;
    size_t paddingSize;
} ID3TagType;

void readV2Tag(FILE*, ID3TagType*);

void remove_id3v2_tag(FILE*, ID3TagType*);

void writteTag(FILE*, ID3TagType*);

void writeID3Tag(FILE *,ID3TagType*);

int storeNextFrame(FILE*, ID3TagType*);

void initID3v2Tag(ID3TagType *);

void freeID3v2Tag(ID3TagType *);

void getRealSizeTag(ID3TagType *);
#endif // ID3V2_H
