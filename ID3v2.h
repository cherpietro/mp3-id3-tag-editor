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
} ID3v2DefaulFrameHeaderType;

typedef struct{
  uint8_t textEncoding;   
  char *mimeType;     //check standar max size
  uint8_t pictureType;    
  char *description;  //check standar max size
  uint8_t *imageData;     
  size_t imageDataSize;   
} ID3v2APICFrame;

void freeAPICFrame(ID3v2APICFrame* );

ID3v2APICFrame* getAPICFrame(uint8_t *, uint32_t );

uint32_t syncsafeToSize(uint8_t *) ;

int readHeader(FILE* , ID3v2HeaderType*);

int readFrameHeader(FILE*, ID3v2DefaulFrameHeaderType *);

int readFrame(FILE*, ID3v2DefaulFrameHeaderType*);
  
#endif // ID3V2_H
