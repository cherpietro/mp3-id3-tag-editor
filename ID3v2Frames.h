#ifndef ID3V2FRAMES_H
#define ID3V2FRAMES_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "TextString.h"

typedef struct {
  char frameId[4];
  uint8_t size[4]; // without header. To full size add 10 bytes
  uint8_t flags[2];
} ID3v2FrameHeaderType;

typedef struct {
  ID3v2FrameHeaderType header;
  uint8_t textEncoding;   
  TextStringType mimeType;
  uint8_t pictureType;    
  TextStringType description;
  uint8_t *imageData;     
  size_t imageDataSize;   
} ID3v2APICFrameType;

typedef struct {
  ID3v2FrameHeaderType header; 
  TextStringType CDTOC;
} ID3v2MCDIFrameType;

typedef struct {
  ID3v2FrameHeaderType header;
  uint8_t textEncoding;
  // char *content;
  TextStringType content;
} ID3v2TXTFrameType;

typedef struct {
  ID3v2FrameHeaderType header;
  uint8_t textEncoding;
  char language[3];
  TextStringType contentDescript;
  TextStringType actualText;
} ID3v2COMMFrameType;

typedef struct {
  ID3v2FrameHeaderType header;
  TextStringType owner;
  TextStringType privateData;
} ID3v2PRIVFrameType;

typedef struct {
  ID3v2FrameHeaderType header;
  TextStringType userEmail;
  uint8_t rating;
  char counter[4];
} ID3v2POPMFrameType;

void FramesV2_storeHeader(FILE*, ID3v2FrameHeaderType*);

void FramesV2_storeAPIC(uint8_t*, uint32_t, ID3v2APICFrameType**);
void FramesV2_freeAPIC(ID3v2APICFrameType**);
void FramesV2_saveAPICImage(ID3v2APICFrameType);
void FramesV2_printAPIC(ID3v2APICFrameType);

//change name to getTXTF
void FramesV2_storeTXTF(FILE*, uint32_t,ID3v2TXTFrameType *);
void FramesV2_printTXTF(ID3v2TXTFrameType);
void FramesV2_freeTXTF(ID3v2TXTFrameType**);

void FramesV2_getCOMM(FILE *, uint32_t , ID3v2COMMFrameType *);
void FramesV2_printCOMM(ID3v2COMMFrameType);
void FramesV2_freeCOMM(ID3v2COMMFrameType**);

void FramesV2_getPRIV(FILE *, uint32_t , ID3v2PRIVFrameType *);
void FramesV2_printPRIV(ID3v2PRIVFrameType);
void FramesV2_freePRIV(ID3v2PRIVFrameType**);

void FramesV2_storeMDCI(FILE*, uint32_t, ID3v2MCDIFrameType**);
void FramesV2_freeMCDI(ID3v2MCDIFrameType**);

void FramesV2_storePOPM(FILE*, uint32_t, ID3v2POPMFrameType**);
void FramesV2_freePOPM(ID3v2POPMFrameType**);

uint32_t FramesV2_getFrameSize(int ,ID3v2FrameHeaderType);

#endif // ID3V2FRAMES_H
