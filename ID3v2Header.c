#include "ID3v2Header.h"
#include "SizeReader.h"

void storeHeader(FILE* mp3FilePointer, ID3v2HeaderType* header) {
    fread(header, sizeof(ID3v2HeaderType), 1, mp3FilePointer);
}

void printTagHeader(ID3v2HeaderType header){
    printf("Version: 2.%d.%d\n",header.version[0],header.version[1]);
    printf("Flag: %u\n",header.flags);
    uint32_t tagSize = syncsafeToSize(header.size);
    printf("Size: %u bytes\n",tagSize);   
}

int isID3v2Tag(ID3v2HeaderType header){
    return header.tag[0] == 'I' && header.tag[1] == 'D' && header.tag[2] == '3';
}

int getTagVersion(ID3v2HeaderType header){
    return header.version[0];
}

int getTagSize(ID3v2HeaderType header){
    return syncsafeToSize(header.size);
}
