#ifndef ID3V2_H
#define ID3V2_H

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "ID3v2Header.h" 
#include "ID3v2Frames.h"


typedef struct {
    ID3v2HeaderType header;
    ID3v2TextFrameType *TALB;
    ID3v2TextFrameType *TPE1; 
    ID3v2TextFrameType *TPE2; 
    ID3v2TextFrameType *TCOM;
    ID3v2TextFrameType *TDRC;
    ID3v2TextFrameType *TPOS;
    ID3v2TextFrameType *TCON;
    ID3v2TextFrameType *TPE3;
    ID3v2TextFrameType *TIT2;
    ID3v2TextFrameType *TRCK;
    ID3v2TextFrameType *TSSE;
    ID3v2APICFrame *APIC;
} ID3TagType;

void initID3v2Tag(ID3TagType *);

void freeID3v2Tag(ID3TagType *);

int readFrame(FILE*, ID3TagType*);

int readFramev2_3(FILE*);

void readV2Tag(FILE*, ID3TagType*);

void printTag(ID3TagType);

uint32_t syncsafeToSize(uint8_t*);

uint32_t sizeFromID3v23(uint8_t*);

#endif // ID3V2_H
