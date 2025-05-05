#include "StoreFrameManager.h"
#include <string.h>


#define STORE_TEXTSTR(frame,txtPtr,txtLen,txtStr)\
    char *txtPtr = (char*) frameContent + index;\
    size_t txtLen = strlen(txtPtr)+1;\
    TxtStr_storeTextString(&frame->txtStr,txtPtr,txtLen);\
    index+=txtLen;


#define STORE_LIST(frameList,frameType,storeFunct)\
    frameType *framePtr = (frameType *)malloc(sizeof(frameType));\
    framePtr->header = header;\
    storeFunct(mp3FilePointer,frameSize, framePtr);\
    ListFramePtr_insertLast(&ID3Tag->frameList,framePtr);

#define STORE_FRAME(frame,frameType,storeFunct)\
    ID3Tag->frame = (frameType *)malloc(sizeof(frameType));\
    storeFunct(mp3FilePointer,frameSize, ID3Tag->frame);\
    ID3Tag->frame->header = header;

void StoreFrame_Header(FILE *mp3FilePointer, ID3v2FrameHeaderType *header){
    fread(header, sizeof(ID3v2FrameHeaderType), 1, mp3FilePointer);
}

int StoreFrame_storeNextFrameInStruct(FILE *mp3FilePointer, ID3TagType *ID3Tag){
    ID3v2FrameHeaderType header;
    StoreFrame_Header(mp3FilePointer,&header);
    uint32_t frameSize = FramesV2_getFrameSize(ID3Tag->header.version[0],header); 
    printf("FrameID: %s %ld\n",header.frameId, ftell(mp3FilePointer));
    if (memcmp(header.frameId, "\0\0\0\0", 4) == 0) return 1;
    else if(strncmp(header.frameId,"TXXX",4)==0){
        STORE_LIST(TXXXFrameList,ID3v2TXXXFrameType,StoreFrame_TXXX);
    }
    else if(strncmp(header.frameId,"T",1)==0){
        STORE_LIST(TXTFrameList,ID3v2TXTFrameType,StoreFrame_TXTF);
    }
    else if(strncmp(header.frameId,"COMM",4)==0){ STORE_LIST(COMMFrameList,ID3v2COMMFrameType,StoreFrame_COMM);}
    else if(strncmp(header.frameId,"PRIV",4)==0){ STORE_LIST(PRIVFrameList,ID3v2PRIVFrameType,StoreFrame_PRIV);}
    else if(strncmp(header.frameId,"APIC",4)==0){ STORE_LIST(APICFrameList,ID3v2APICFrameType,StoreFrame_APIC);}
    else if(strncmp(header.frameId,"POPM",4)==0){ STORE_LIST(POPMFrameList,ID3v2POPMFrameType,StoreFrame_POPM);}
    else if(strncmp(header.frameId,"WXXX",4)==0){
        STORE_LIST(WXXXFrameList,ID3v2WXXXFrameType,StoreFrame_WXXX);
    }
    else if(strncmp(header.frameId,"W",1)==0){ STORE_LIST(WWWFrameList,ID3v2WWWFrameType,StoreFrame_WWWF);}
    else if(strncmp(header.frameId,"MCDI",4)==0){ STORE_FRAME(MCDI,ID3v2DefaultFrameType,StoreFrame_DefaultFrame);}
    //DEFAULT FRAMES
    else if(strncmp(header.frameId,"IPLS",4)==0){ STORE_FRAME(IPLS,ID3v2IPLSFrameType,StoreFrame_storeIPLS);}
    else if(strncmp(header.frameId,"SYTC",4)==0){ STORE_FRAME(SYTC,ID3v2DefaultFrameType,StoreFrame_DefaultFrame);}
    else if(strncmp(header.frameId,"USER",4)==0){ STORE_FRAME(USER,ID3v2USERFrameType,StoreFrame_USER);}
    else if(strncmp(header.frameId,"OWNE",4)==0){ STORE_FRAME(OWNE,ID3v2DefaultFrameType,StoreFrame_DefaultFrame);}
    else if(strncmp(header.frameId,"PCNT",4)==0){ STORE_FRAME(PCNT,ID3v2PCNTFrameType,StoreFrame_PCNT);}
    else if(strncmp(header.frameId,"RVRB",4)==0){ STORE_FRAME(RVRB,ID3v2RVRBFrameType,StoreFrame_RVRB);}
    else if(strncmp(header.frameId,"EQUA",4)==0){ STORE_FRAME(EQUA,ID3v2DefaultFrameType,StoreFrame_DefaultFrame);}
    else if(strncmp(header.frameId,"MLLT",4)==0){ STORE_FRAME(MLLT,ID3v2DefaultFrameType,StoreFrame_DefaultFrame);}
    else if(strncmp(header.frameId,"ETCO",4)==0){ STORE_FRAME(ETCO,ID3v2DefaultFrameType,StoreFrame_DefaultFrame);}
    else if(strncmp(header.frameId,"RVAD",4)==0){ STORE_FRAME(RVAD,ID3v2DefaultFrameType,StoreFrame_DefaultFrame);}
    else if(strncmp(header.frameId,"POSS",4)==0){ STORE_FRAME(POSS,ID3v2DefaultFrameType,StoreFrame_DefaultFrame);}
    else if(strncmp(header.frameId,"COMR",4)==0){ STORE_FRAME(COMR,ID3v2DefaultFrameType,StoreFrame_DefaultFrame);}
    //default Lists
    else if(strncmp(header.frameId,"UFID",4)==0){STORE_LIST(UFIDFrameList,ID3v2DefaultFrameType,StoreFrame_DefaultFrame);}
    else if(strncmp(header.frameId,"USLT",4)==0){STORE_LIST(USLTFrameList,ID3v2DefaultFrameType,StoreFrame_DefaultFrame);}
    else if(strncmp(header.frameId,"SYLT",4)==0){STORE_LIST(SYLTFrameList,ID3v2DefaultFrameType,StoreFrame_DefaultFrame);}
    else if(strncmp(header.frameId,"GEOB",4)==0){STORE_LIST(GEOBFrameList,ID3v2DefaultFrameType,StoreFrame_DefaultFrame);}
    else if(strncmp(header.frameId,"LINK",4)==0){STORE_LIST(LINKFrameList,ID3v2DefaultFrameType,StoreFrame_DefaultFrame);}
    else if(strncmp(header.frameId,"AENC",4)==0){STORE_LIST(AENCFrameList,ID3v2DefaultFrameType,StoreFrame_DefaultFrame);}
    else if(strncmp(header.frameId,"ENCR",4)==0){STORE_LIST(ENCRFrameList,ID3v2DefaultFrameType,StoreFrame_DefaultFrame);}
    else if(strncmp(header.frameId,"GRID",4)==0){STORE_LIST(GRIDFrameList,ID3v2DefaultFrameType,StoreFrame_DefaultFrame);}
    else{
        printf("NOT SUPPORTED TAG %s: %ld\nSize: %d\n", header.frameId,ftell(mp3FilePointer),frameSize);
        uint8_t *buffer = (uint8_t *)malloc(frameSize);
        fread(buffer, frameSize, 1, mp3FilePointer);
        free(buffer);
        return -1;
    }
    return 0;
}

void StoreFrame_APIC(FILE *mp3FilePointer, uint32_t frameSize,ID3v2APICFrameType* APIC){
    uint8_t *frameContent = (uint8_t *)malloc(frameSize);
    fread(frameContent, frameSize, 1, mp3FilePointer);
    size_t index = 0;
    APIC->textEncoding = frameContent[index++];
    STORE_TEXTSTR(APIC,mimeTypePtr,mimeTypeLen,mimeType);
    APIC->pictureType = frameContent[index++];
    STORE_TEXTSTR(APIC,descPtr,descLen,description);
    APIC->imageDataSize = frameSize - index;
    APIC->imageData = (uint8_t *)malloc(APIC->imageDataSize);
    memcpy(APIC->imageData, frameContent + index, APIC->imageDataSize);
    free(frameContent);
}

void StoreFrame_TXTF(FILE *mp3FilePointer, uint32_t frameSize, ID3v2TXTFrameType *TXTF){
    uint8_t *frameContent = (uint8_t *)malloc(frameSize);
    fread(frameContent, frameSize, 1, mp3FilePointer);
    TXTF->textEncoding = frameContent[0];
    char *contentPtr = (char *)frameContent + 1;
    TxtStr_storeTextString(&TXTF->content,contentPtr, frameSize-1);
    free(frameContent);
}

void StoreFrame_COMM(FILE *mp3FilePointer, uint32_t frameSize, ID3v2COMMFrameType *COMM){
    uint8_t *frameContent = (uint8_t *)malloc(frameSize);
    fread(frameContent, frameSize, 1, mp3FilePointer);
    size_t index = 0;
    COMM->textEncoding = frameContent[index++];
    COMM->language[0] = frameContent[index++];
    COMM->language[1] = frameContent[index++];
    COMM->language[2] = frameContent[index++];
    STORE_TEXTSTR(COMM,descPtr,descSize,contentDescript);

    char *contentPtr = (char*) frameContent + index;
    size_t contentSize = frameSize-index;
    TxtStr_storeTextString(&COMM->actualText,contentPtr,contentSize);
    free(frameContent);
}

void StoreFrame_MDCI(FILE *mp3FilePointer, uint32_t frameSize,ID3v2MCDIFrameType* MDCI){
    uint8_t *frameContent = (uint8_t *)malloc(frameSize);
    fread(frameContent, frameSize, 1, mp3FilePointer);
    char *ptr = (char *)frameContent;
    TxtStr_storeTextString(&(MDCI)->CDTOC,ptr, frameSize);
    free(frameContent);
}

void StoreFrame_POPM(FILE *mp3FilePointer, uint32_t frameSize,ID3v2POPMFrameType* POPM){
    uint8_t *frameContent = (uint8_t *)malloc(frameSize);
    fread(frameContent, frameSize, 1, mp3FilePointer);

    size_t index = 0;
    char *emailPtr = (char *)frameContent;
    size_t emailSize = strlen(emailPtr) + 1;
    TxtStr_storeTextString(&(POPM)->userEmail,emailPtr,emailSize);
    index += emailSize;
    char *ratingPtr = (char *)frameContent + index ; 
    (POPM)->rating = ratingPtr[0];
    char *counterPtr = (char *)frameContent + index +1; 
    // memcpy(&(POPM)->counter,counterPtr,4);
    (POPM)->counter[0] = counterPtr[0];
    (POPM)->counter[1] = counterPtr[1];
    (POPM)->counter[2] = counterPtr[2];
    (POPM)->counter[3] = counterPtr[3];
    free(frameContent);
}

void StoreFrame_PCNT(FILE* mp3FilePointer, uint32_t frameSize, ID3v2PCNTFrameType *PCNT){
    uint8_t *frameContent = (uint8_t *)malloc(frameSize);
    fread(frameContent, frameSize, 1, mp3FilePointer);

    char *counterPtr = (char *)frameContent;
    (PCNT)->counter[0] = counterPtr[0];
    (PCNT)->counter[1] = counterPtr[1];
    (PCNT)->counter[2] = counterPtr[2];
    (PCNT)->counter[3] = counterPtr[3];
    free(frameContent);
}

void StoreFrame_RVRB(FILE* mp3FilePointer, uint32_t frameSize, ID3v2RVRBFrameType *RVRB){
    uint8_t *frameContent = (uint8_t *)malloc(frameSize);
    fread(frameContent, frameSize, 1, mp3FilePointer);
    RVRB->left = (frameContent[0] << 8) | frameContent[1];
    RVRB->right = (frameContent[2] << 8) | frameContent[3];
    RVRB->bouncesL = frameContent[4];
    RVRB->bouncesR = frameContent[5];
    RVRB->feedBackLL = frameContent[6];
    RVRB->feedBackLLR = frameContent[7];
    RVRB->feedBackRR = frameContent[8];
    RVRB->feedBackRL = frameContent[9];
    RVRB->premixLR = frameContent[10];
    RVRB->premixRL = frameContent[11];    
    free(frameContent);
}

void StoreFrame_WWWF(FILE* mp3FilePointer, uint32_t frameSize,ID3v2WWWFrameType *WWWF){
    uint8_t *frameContent = (uint8_t *)malloc(frameSize);
    fread(frameContent, frameSize, 1, mp3FilePointer);
    char *urlPtr = (char *)frameContent;
    TxtStr_storeTextString(&(WWWF)->url,urlPtr,frameSize);
    free(frameContent);
}

void StoreFrame_TXXX(FILE *mp3FilePointer, uint32_t frameSize,ID3v2TXXXFrameType *TXXX){
    uint8_t *frameContent = (uint8_t *)malloc(frameSize);
    fread(frameContent, frameSize, 1, mp3FilePointer);
    size_t index = 0;
    TXXX->textEncoding = frameContent[index++];
    STORE_TEXTSTR(TXXX,descPtr,descSize,description);
    STORE_TEXTSTR(TXXX,valuePtr,valueSize,value);
    free(frameContent);
}
void StoreFrame_WXXX(FILE *mp3FilePointer, uint32_t frameSize,ID3v2WXXXFrameType *WXXX){
    uint8_t *frameContent = (uint8_t *)malloc(frameSize);
    fread(frameContent, frameSize, 1, mp3FilePointer);
    size_t index = 0;
    WXXX->textEncoding = frameContent[index++];
    STORE_TEXTSTR(WXXX,descPtr,descSize,description);
    STORE_TEXTSTR(WXXX,urlPtr,urlSize,url);
    free(frameContent);
}

void StoreFrame_PRIV(FILE *mp3FilePointer, uint32_t frameSize, ID3v2PRIVFrameType *frame){
    uint8_t *frameContent = (uint8_t *)malloc(frameSize);
    fread(frameContent, frameSize, 1, mp3FilePointer);

    size_t index = 0;
    char *ownerPtr = (char *)frameContent;
    size_t descSize = strlen(ownerPtr)+1; //description ALWAYS has to have \0
    TxtStr_storeTextString(&frame->owner,ownerPtr, descSize);
    index += descSize;

    char *privateDataPtr = (char *)frameContent + index ; 
    size_t contentSize = frameSize - index ; //Check this operation
    TxtStr_storeTextString(&frame->privateData,privateDataPtr, contentSize);
    free(frameContent);
}
void StoreFrame_DefaultFrame(FILE* mp3FilePointer, uint32_t frameSize, ID3v2DefaultFrameType *DefaultFrame){
    (DefaultFrame)->frameData = (uint8_t *)malloc(frameSize);
    fread((DefaultFrame)->frameData, frameSize, 1, mp3FilePointer);
    DefaultFrame->size = frameSize;
}

void StoreFrame_storeIPLS(FILE* mp3FilePointer, uint32_t frameSize, ID3v2IPLSFrameType *IPLS){
    IPLS = (ID3v2IPLSFrameType *)malloc(sizeof(ID3v2IPLSFrameType));
    uint8_t *frameContent = (uint8_t *)malloc(frameSize);
    fread(frameContent, frameSize, 1, mp3FilePointer);

    (IPLS)->textEncoding = frameContent[0];
    char *peopleLstPtr = (char *)frameContent + 1;
    TxtStr_storeTextString(&(IPLS)->peopeList,peopleLstPtr,frameSize -1);
    free(frameContent);
}

void StoreFrame_USER(FILE* mp3FilePointer, uint32_t frameSize, ID3v2USERFrameType *USER){
    USER = (ID3v2USERFrameType *)malloc(sizeof(ID3v2USERFrameType));
    uint8_t *frameContent = (uint8_t *)malloc(frameSize);
    fread(frameContent, frameSize, 1, mp3FilePointer);
    size_t index = 0;
    USER->textEncoding = frameContent[index++];
    USER->language[0] = frameContent[index++];
    USER->language[1] = frameContent[index++];
    USER->language[2] = frameContent[index++];
    char *contentPtr = (char*) frameContent + index;
    size_t contentSize = frameSize-index;
    TxtStr_storeTextString(&USER->actualText,contentPtr,contentSize);
    free(frameContent);
}
