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

void ID3v2_init(ID3TagType *);

void ID3v2_initID3v2Tag(ID3TagType *);

void ID3v2_storeTagInStruct(FILE*, ID3TagType*);

void ID3v2_removeTagFromFile(FILE*);

int ID3v2_storeNextFrameInStruct(FILE*, ID3TagType*);

void ID3v2_getTagSizeOfTheStruct(ID3TagType *);
#endif // ID3V2_H
