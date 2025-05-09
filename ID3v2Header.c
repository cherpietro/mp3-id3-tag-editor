#include "ID3v2Header.h"
#include "SizeReader.h"
#include "stdbool.h"

void HeaderV2_storeHeader(FILE* mp3FilePointer, ID3v2HeaderType* header) {
    fread(header, sizeof(ID3v2HeaderType), 1, mp3FilePointer);
}

void HeaderV2_printTagHeader(ID3v2HeaderType header){
    printf("----TAG HEADER----\n");
    printf("Version: 2.%d.%d\n",header.version[0],header.version[1]);
    printf("Flag: %u\n",header.flags);
    uint32_t tagSize = syncsafeToSize(header.size);
    printf("Size: %u bytes\n",tagSize); 
    printf("\n");
}

int HeaderV2_isID3v2Tag(ID3v2HeaderType header){
    return ( header.tag[0] == 'I' && header.tag[1] == 'D' && header.tag[2] == '3' && (HeaderV2_getTagVersion(header) == 3 || HeaderV2_getTagVersion(header) == 4));
}

int HeaderV2_isID3(ID3v2HeaderType header){
    return ( header.tag[0] == 'I' && header.tag[1] == 'D' && header.tag[2] == '3' );
}

int HeaderV2_getTagVersion(ID3v2HeaderType header){
    return header.version[0];
}

int HeaderV2_getTagSize(ID3v2HeaderType header){
    return syncsafeToSize(header.size);
}

void HeaderV2_updateTagSize(ID3v2HeaderType *header,uint32_t newSize){
    saveSizeToSyncsafe(newSize,header->size);
}

void HeaderV2_init(ID3v2HeaderType *header){
    header->tag[0] = 'I';
    header->tag[1] = 'D';
    header->tag[2] = '3';
    header->version[0] = 4;
    header->version[1] = 0;
    header->flags = 0;
}
