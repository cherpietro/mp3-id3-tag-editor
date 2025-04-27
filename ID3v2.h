#ifndef ID3V2_H
#define ID3V2_H

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "ID3v2Header.h" 
#include "ID3v2Frames.h"
#include "TextFrameList.h"
#include "COMMFrameList.h"
#include "PRIVFrameList.h"

typedef struct {
    ID3v2HeaderType header;
    TextFrameList textFrameList;
    COMMFrameList COMMFrameList;
    PRIVFrameList PRIVFrameList;
    ID3v2MCDIFrameType *MCDI;
    ID3v2POPMFrameType *POPM; //LIST
    ID3v2APICFrame *APIC;
    size_t paddingSize;
} ID3TagType;

void ID3v2_init(ID3TagType *);

void ID3v2_free(ID3TagType *);

void printTag(ID3TagType *);

void ID3v2_storeTagInStruct(char*, ID3TagType*);

void ID3v2_removeTagFromFile(char*);

void ID3v2_writteTagIntoFile(char *, ID3TagType *);

void ID3v2_saveAPICImage(ID3TagType *);

int ID3v2_storeNextFrameInStruct(FILE*, ID3TagType*);

void ID3v2_getTagSizeOfTheStruct(ID3TagType *);

#endif // ID3V2_H
