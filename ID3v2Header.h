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

void readHeader(FILE* mp3FilePointer, ID3v2HeaderType* header);

#endif // ID3V2HEADER_H
