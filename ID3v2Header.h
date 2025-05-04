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

void HeaderV2_storeHeader(FILE* , ID3v2HeaderType* );

void HeaderV2_printTagHeader(ID3v2HeaderType );

int HeaderV2_isID3v2Tag(ID3v2HeaderType);

int HeaderV2_isID3(ID3v2HeaderType );

int HeaderV2_getTagVersion(ID3v2HeaderType );

int HeaderV2_getTagSize(ID3v2HeaderType header);

void HeaderV2_updateTagSize(ID3v2HeaderType *,uint32_t );

void HeaderV2_init(ID3v2HeaderType *);

#endif // ID3V2HEADER_H
