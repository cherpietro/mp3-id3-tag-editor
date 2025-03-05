#ifndef ID3V2_H
#define ID3V2_H

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
/*ID3v2 Standar*/
// ID3v2/file identifier   //"ID3" 
// ID3v2 version           //$04 00
// ID3v2 flags             //%abc00000
// ID3v2 size              //4 * %0xxxxxxx
//$ means bytes- %means the hole is 1 byte

typedef struct{
  char tag[3];
  uint8_t version[2];
  uint8_t flags; //¿not used?
  uint8_t size[4];
} ID3v2HeaderType;

typedef struct{
  char frameId[4];
  uint8_t size[4];
  uint8_t flags[2];
} ID3v2FrameHeaderType;

typedef struct{
  ID3v2FrameHeaderType header;
  uint8_t textEncoding;   
  char *mimeType;     //check standar max size
  uint8_t pictureType;    
  char *description;  //check standar max size
  uint8_t *imageData;     
  size_t imageDataSize;   
} ID3v2APICFrame;

typedef struct{
  ID3v2FrameHeaderType header;
  uint8_t textEncoding;
  char *content;
}ID3v2TextFrameType;


typedef struct{
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

void freeAPICFrame(ID3v2APICFrame* );

ID3v2APICFrame* getAPICFrame(uint8_t *, uint32_t );

uint32_t syncsafeToSize(uint8_t *) ;

uint32_t sizeFromID3v23(uint8_t *size);

int readHeader(FILE* , ID3v2HeaderType*);

int readFrameHeader(FILE*, ID3v2FrameHeaderType *);

int readFrame(FILE*, ID3TagType*);

int readFramev2_3(FILE *mp3FilePointer, ID3TagType *ID3Tag);

void storeTextFrameContet(FILE *, ID3v2FrameHeaderType , uint32_t, ID3v2TextFrameType **);

#endif // ID3V2_H
