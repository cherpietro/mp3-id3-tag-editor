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
    TextStringType peopeList;
} ID3v2IPLSFrameType;

typedef struct {
    ID3v2FrameHeaderType header;
    uint8_t textEncoding;
    // char *content;
    TextStringType content;
} ID3v2TXTFrameType;
typedef struct {
    ID3v2FrameHeaderType header;
    uint8_t textEncoding;
    // char *content;
    TextStringType description;
    TextStringType value;
} ID3v2TXXXFrameType;

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

typedef struct {
    ID3v2FrameHeaderType header;
    char counter[4];
} ID3v2PCNTFrameType;


typedef struct {
    ID3v2FrameHeaderType header;
    TextStringType url;
} ID3v2WWWFrameType;

typedef struct {
    ID3v2FrameHeaderType header;
    uint16_t left;
    uint16_t right;
    uint8_t bouncesL;
    uint8_t bouncesR;
    uint8_t feedBackLL;
    uint8_t feedBackLLR;
    uint8_t feedBackRR;
    uint8_t feedBackRL;
    uint8_t premixLR;
    uint8_t premixRL;
} ID3v2RVRBFrameType;

typedef struct {
    ID3v2FrameHeaderType header;
    uint8_t textEncoding;
    char language[3];
    TextStringType actualText;
} ID3v2USERFrameType;

typedef struct {
    ID3v2FrameHeaderType header;
    uint8_t textEncoding;
    TextStringType description;
    TextStringType url;
} ID3v2WXXXFrameType;

typedef struct {
    ID3v2FrameHeaderType header;
    uint8_t *frameData;
    size_t size;
} ID3v2DefaultFrameType;


uint32_t FramesV2_getFrameSize(int ,ID3v2FrameHeaderType);
void FramesV2_updateFrameSize(int , ID3v2FrameHeaderType *,uint32_t );

void FramesV2_freeAPIC(ID3v2APICFrameType**);




void FramesV2_freeTXTF(ID3v2TXTFrameType**);
void FramesV2_freeTXXX(ID3v2TXXXFrameType **);


void FramesV2_freeWWWF(ID3v2WWWFrameType**);
void FramesV2_freeWXXX(ID3v2WXXXFrameType**);


void FramesV2_freeCOMM(ID3v2COMMFrameType**);

void FramesV2_freePRIV(ID3v2PRIVFrameType**);
void FramesV2_freeMCDI(ID3v2MCDIFrameType**);
void FramesV2_freePOPM(ID3v2POPMFrameType**);
void FramesV2_freeDefaultFrame(ID3v2DefaultFrameType**);
void FramesV2_freePCNT(ID3v2PCNTFrameType**);
void FramesV2_freeIPLS(ID3v2IPLSFrameType**);
void FramesV2_freeUSER(ID3v2USERFrameType **);
void FramesV2_freeRVRB(ID3v2RVRBFrameType **);
#endif // ID3V2FRAMES_H
