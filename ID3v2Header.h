#ifndef ID3V2HEADER_H
#define ID3V2HEADER_H

#include <stdint.h>
#include <stdio.h>

typedef struct {
    char tag[3];
    uint8_t version[2];
    uint8_t flags; 
    uint8_t size[4];
} ID3v2HeaderType;

void storeHeader(FILE* , ID3v2HeaderType* );

void printTagHeader(ID3v2HeaderType );

int isID3v2Tag(ID3v2HeaderType);

int getTagVersion(ID3v2HeaderType );

int getTagSize(ID3v2HeaderType header);

#endif // ID3V2HEADER_H
