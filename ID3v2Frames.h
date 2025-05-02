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
  TextStringType CDTOC; //change to uint8_t* CDTOC
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
  char counter[4]; //MAYBE COUNTER CAN INCREASE
} ID3v2POPMFrameType;

typedef struct {
  ID3v2FrameHeaderType header;
  uint8_t textEncoding;
  TextStringType peopeList;
} ID3v2IPLSFrameType;

typedef struct {
  ID3v2FrameHeaderType header;
  uint8_t timestampFormat;
  uint8_t* tempoData;
} ID3v2SYTCFrameType;

typedef struct {
  ID3v2FrameHeaderType header;
  uint8_t textEncoding;
  char language[3];
  TextStringType actualText;
} ID3v2USERFrameType;

typedef struct {
  ID3v2FrameHeaderType header;
  uint8_t textEncoding;
  TextStringType pricePayed;
  char dateOfPurch[8];
  TextStringType seller;
} ID3v2OWNEFrameType;

typedef struct {
  ID3v2FrameHeaderType header;
  TextStringType counter;
} ID3v2PCNTFrameType;

typedef struct {
  ID3v2FrameHeaderType header;
  TextStringType url;
} ID3v2WWWFrameType;

typedef struct {
  ID3v2FrameHeaderType header;
  uint8_t textEncoding;
  TextStringType description;
  TextStringType url;
} ID3v2WXXXFrameType;

typedef struct {
  ID3v2FrameHeaderType header;
  uint8_t *frameData;
} ID3v2DefaultFrameType;


void FramesV2_storeHeader(FILE*, ID3v2FrameHeaderType*);
uint32_t FramesV2_getFrameSize(int ,ID3v2FrameHeaderType);
void FramesV2_updateFrameSize(int , ID3v2FrameHeaderType *,uint32_t );

void FramesV2_storeAPIC(FILE*, uint32_t, ID3v2APICFrameType*);
void FramesV2_saveAPICImage(ID3v2APICFrameType);
void FramesV2_printAPIC(ID3v2APICFrameType);
void FramesV2_freeAPIC(ID3v2APICFrameType**);
void FramesV2_ModifyAPIC(uint8_t, ID3v2APICFrameType*);

void FramesV2_storeTXTF(FILE*, uint32_t,ID3v2TXTFrameType *);
ID3v2TXTFrameType* FramesV2_getTXXX();
ID3v2TXTFrameType* FramesV2_getTXTF(char *,int);
void FramesV2_printTXTF(ID3v2TXTFrameType);
void FramesV2_freeTXTF(ID3v2TXTFrameType**);
bool FramesV2_validTextFrameId(char *);
void FramesV2_ModifyTXTF(uint8_t, ID3v2TXTFrameType *);

void FramesV2_storeCOMM(FILE *, uint32_t , ID3v2COMMFrameType *);
void FramesV2_printCOMM(ID3v2COMMFrameType);
void FramesV2_freeCOMM(ID3v2COMMFrameType**);

void FramesV2_storePRIV(FILE *, uint32_t , ID3v2PRIVFrameType *);
void FramesV2_printPRIV(ID3v2PRIVFrameType);
void FramesV2_freePRIV(ID3v2PRIVFrameType**);

void FramesV2_storeMDCI(FILE*, uint32_t, ID3v2MCDIFrameType**);
void FramesV2_printMDCI(ID3v2MCDIFrameType);
void FramesV2_freeMCDI(ID3v2MCDIFrameType**);

void FramesV2_storePOPM(FILE*, uint32_t, ID3v2POPMFrameType*);
void FramesV2_printPOPM(ID3v2POPMFrameType);
void FramesV2_freePOPM(ID3v2POPMFrameType**);

void FramesV2_storeIPLS(FILE*, uint32_t, ID3v2IPLSFrameType**);
void FramesV2_printIPLS(ID3v2IPLSFrameType);
void FramesV2_freeIPLS(ID3v2IPLSFrameType**);

void FramesV2_storeSYTC(FILE*, uint32_t, ID3v2SYTCFrameType**);
void FramesV2_printSYTC(ID3v2SYTCFrameType);
void FramesV2_freeSYTC(ID3v2SYTCFrameType**);

void FramesV2_storeUSER(FILE*, uint32_t, ID3v2USERFrameType**);
void FramesV2_printUSER(ID3v2USERFrameType);
void FramesV2_freeUSER(ID3v2USERFrameType**);

void FramesV2_storeOWNE(FILE*, uint32_t, ID3v2OWNEFrameType**);
void FramesV2_printOWNE(ID3v2OWNEFrameType);
void FramesV2_freeOWNE(ID3v2OWNEFrameType**);

void FramesV2_storePCNT(FILE*, uint32_t, ID3v2PCNTFrameType**);
void FramesV2_printPCNT(ID3v2PCNTFrameType);
void FramesV2_freePCNT(ID3v2PCNTFrameType**);

void FramesV2_storeDefaultFrame(FILE*, uint32_t, ID3v2DefaultFrameType**);
void FramesV2_printDefaultFrame(ID3v2DefaultFrameType);
void FramesV2_freeDefaultFrame(ID3v2DefaultFrameType**);

void FramesV2_storeWWWF(FILE*, uint32_t,ID3v2WWWFrameType *);
void FramesV2_printWWWF(ID3v2WWWFrameType);
void FramesV2_freeWWWF(ID3v2WWWFrameType**);

void FramesV2_storeWXXX(FILE*, uint32_t,ID3v2WXXXFrameType *);
void FramesV2_printWXXX(ID3v2WXXXFrameType);
void FramesV2_freeWXXX(ID3v2WXXXFrameType**);
#endif // ID3V2FRAMES_H
