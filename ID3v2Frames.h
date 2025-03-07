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


void freeAPICFrame(ID3v2APICFrame*);

void printAPICFrame(ID3v2APICFrame);

void printTextFrame(ID3v2TextFrameType);

void printTextFrame(ID3v2TextFrameType);

ID3v2APICFrame* getAPICFrame(uint8_t*, uint32_t);

void storeTextFrameContet(FILE*, ID3v2FrameHeaderType, uint32_t, ID3v2TextFrameType**);

int readFrameHeader(FILE*, ID3v2FrameHeaderType*);

#endif // ID3V2FRAMES_H