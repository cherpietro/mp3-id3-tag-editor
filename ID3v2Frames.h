#ifndef ID3V2FRAMES_H
#define ID3V2FRAMES_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct {
  char frameId[4];
  uint8_t size[4];
  uint8_t flags[2];
} ID3v2FrameHeaderType;

typedef struct {
  ID3v2FrameHeaderType header;
  uint8_t textEncoding;   
  char *mimeType;
  uint8_t pictureType;    
  char *description;
  uint8_t *imageData;     
  size_t imageDataSize;   
} ID3v2APICFrame;

typedef struct {
  ID3v2FrameHeaderType header;
  uint8_t textEncoding;
  char *content;
} ID3v2TextFrameType;

typedef struct {
  ID3v2FrameHeaderType header;
  uint8_t textEncoding;
  char language[3];
  char *contentDescript;
  char *actualText;
} ID3v2COMMFrameType;

void FramesV2_storeHeader(FILE*, ID3v2FrameHeaderType*);

void FramesV2_freeAPIC(ID3v2APICFrame*);
void FramesV2_printAPIC(ID3v2APICFrame);
void FramesV2_storeAPIC(uint8_t*, uint32_t, ID3v2APICFrame**);

void FramesV2_getTXTF(FILE*, uint32_t,ID3v2TextFrameType *);
void FramesV2_storeTXTF(FILE*, ID3v2FrameHeaderType, uint32_t, ID3v2TextFrameType**);
void FramesV2_printTXTF(ID3v2TextFrameType);

void FramesV2_getCOMM(FILE *, uint32_t , ID3v2COMMFrameType *);

/*this should be on header?*/
uint32_t FramesV2_getSize_V2_4(ID3v2FrameHeaderType);
uint32_t FramesV2_getSize_V2_3(ID3v2FrameHeaderType);
/**/

#endif // ID3V2FRAMES_H
