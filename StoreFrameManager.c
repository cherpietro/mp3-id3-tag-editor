#include "StoreFrameManager.h"
#include <string.h>

#define STORE_LIST(frameList,frameType,storeFunct)\
    frameType *framePtr = (frameType *)malloc(sizeof(frameType));\
    framePtr->header = header;\
    storeFunct(mp3FilePointer,frameSize, framePtr);\
    ListFramePtr_insertLast(&ID3Tag->frameList,framePtr);

#define STORE_FRAME(frame,frameType,storeFunct)\
    ID3Tag->frame = (frameType *)malloc(sizeof(frameType));\
    storeFunct(mp3FilePointer,frameSize, ID3Tag->frame);\
    ID3Tag->frame->header = header;

bool StoreFrame_storeNextFrameInStruct(FILE *mp3FilePointer, ID3TagType *ID3Tag){
    ID3v2FrameHeaderType header;
    FramesV2_storeHeader(mp3FilePointer,&header);
    uint32_t frameSize = FramesV2_getFrameSize(ID3Tag->header.version[0],header); 
    printf("FrameID: %s\n",header.frameId);
    if (memcmp(header.frameId, "\0\0\0\0", 4) == 0) return true;
    else if(strncmp(header.frameId,"T",1)==0){
        STORE_LIST(TXTFrameList,ID3v2TXTFrameType,FramesV2_storeTXTF);
    }
    else if(strncmp(header.frameId,"COMM",4)==0){ STORE_LIST(COMMFrameList,ID3v2COMMFrameType,FramesV2_storeCOMM);}
    else if(strncmp(header.frameId,"PRIV",4)==0){ STORE_LIST(PRIVFrameList,ID3v2PRIVFrameType,FramesV2_storePRIV);}
    else if(strncmp(header.frameId,"APIC",4)==0){ STORE_LIST(APICFrameList,ID3v2APICFrameType,FramesV2_storeAPIC);}
    else if(strncmp(header.frameId,"POPM",4)==0){ STORE_LIST(POPMFrameList,ID3v2POPMFrameType,FramesV2_storePOPM);}
    else if(strncmp(header.frameId,"WXXX",4)==0){
        printf("NOT TESTED TAG %s: %ld\nSize: %d\n", header.frameId,ftell(mp3FilePointer),frameSize);
        STORE_LIST(WXXXFrameList,ID3v2WXXXFrameType,FramesV2_storeWXXX);
    }
    else if(strncmp(header.frameId,"W",1)==0){
        STORE_LIST(WWWFrameList,ID3v2WWWFrameType,FramesV2_storeWWWF);
        printf("NOT TESTED TAG %s: %ld\nSize: %d\n", header.frameId,ftell(mp3FilePointer),frameSize);
    }
    else if(strncmp(header.frameId,"MCDI",4)==0){
        printf("NOT TESTED TAG %s: %ld\nSize: %d\n", header.frameId,ftell(mp3FilePointer),frameSize);
        STORE_FRAME(MCDI,ID3v2MCDIFrameType,FramesV2_storeMDCI);
    }
    //DEFAULT FRAMES
    else if(strncmp(header.frameId,"IPLS",4)==0){
        STORE_FRAME(IPLS,ID3v2DefaultFrameType,FramesV2_storeDefaultFrame);
        printf("NOT TESTED TAG %s: %ld\nSize: %d\n", header.frameId,ftell(mp3FilePointer),frameSize);
    }
    else if(strncmp(header.frameId,"SYTC",4)==0){
        STORE_FRAME(SYTC,ID3v2DefaultFrameType,FramesV2_storeDefaultFrame);
        printf("NOT TESTED TAG %s: %ld\nSize: %d\n", header.frameId,ftell(mp3FilePointer),frameSize);
    }
    else if(strncmp(header.frameId,"USER",4)==0){
        STORE_FRAME(USER,ID3v2DefaultFrameType,FramesV2_storeDefaultFrame);
        printf("NOT TESTED TAG %s: %ld\nSize: %d\n", header.frameId,ftell(mp3FilePointer),frameSize);
    }
    else if(strncmp(header.frameId,"OWNE",4)==0){
        STORE_FRAME(OWNE,ID3v2DefaultFrameType,FramesV2_storeDefaultFrame);
        printf("NOT TESTED TAG %s: %ld\nSize: %d\n", header.frameId,ftell(mp3FilePointer),frameSize);
    }
    else if(strncmp(header.frameId,"PCNT",4)==0){
        STORE_FRAME(PCNT,ID3v2DefaultFrameType,FramesV2_storeDefaultFrame);
        printf("NOT TESTED TAG %s: %ld\nSize: %d\n", header.frameId,ftell(mp3FilePointer),frameSize);
    }
    else if(strncmp(header.frameId,"RVRB",4)==0){
        STORE_FRAME(RVRB,ID3v2DefaultFrameType,FramesV2_storeDefaultFrame);
        printf("NOT TESTED TAG %s: %ld\nSize: %d\n", header.frameId,ftell(mp3FilePointer),frameSize);
    }
    else if(strncmp(header.frameId,"EQUA",4)==0){
        STORE_FRAME(EQUA,ID3v2DefaultFrameType,FramesV2_storeDefaultFrame);
        printf("NOT TESTED TAG %s: %ld\nSize: %d\n", header.frameId,ftell(mp3FilePointer),frameSize);
    }
    else if(strncmp(header.frameId,"MLLT",4)==0){
        STORE_FRAME(MLLT,ID3v2DefaultFrameType,FramesV2_storeDefaultFrame);
        printf("NOT TESTED TAG %s: %ld\nSize: %d\n", header.frameId,ftell(mp3FilePointer),frameSize);
    }
    else if(strncmp(header.frameId,"ETCO",4)==0){
        STORE_FRAME(ETCO,ID3v2DefaultFrameType,FramesV2_storeDefaultFrame);
        printf("NOT TESTED TAG %s: %ld\nSize: %d\n", header.frameId,ftell(mp3FilePointer),frameSize);
    }
    else if(strncmp(header.frameId,"RVAD",4)==0){
        STORE_FRAME(RVAD,ID3v2DefaultFrameType,FramesV2_storeDefaultFrame);
        printf("NOT TESTED TAG %s: %ld\nSize: %d\n", header.frameId,ftell(mp3FilePointer),frameSize);
    }
    else if(strncmp(header.frameId,"POSS",4)==0){
        STORE_FRAME(POSS,ID3v2DefaultFrameType,FramesV2_storeDefaultFrame);
        printf("NOT TESTED TAG %s: %ld\nSize: %d\n", header.frameId,ftell(mp3FilePointer),frameSize);
    }
    else if(strncmp(header.frameId,"COMR",4)==0){
        STORE_FRAME(COMR,ID3v2DefaultFrameType,FramesV2_storeDefaultFrame);
        printf("NOT TESTED TAG %s: %ld\nSize: %d\n", header.frameId,ftell(mp3FilePointer),frameSize);
    }
    //default Lists
    else if(strncmp(header.frameId,"UFID",4)==0){
        STORE_LIST(UFIDFrameList,ID3v2DefaultFrameType,FramesV2_storeDefaultFrame);
        printf("NOT TESTED TAG %s: %ld\nSize: %d\n", header.frameId,ftell(mp3FilePointer),frameSize);
    }
    else if(strncmp(header.frameId,"USLT",4)==0){
        STORE_LIST(USLTFrameList,ID3v2DefaultFrameType,FramesV2_storeDefaultFrame);
        printf("NOT TESTED TAG %s: %ld\nSize: %d\n", header.frameId,ftell(mp3FilePointer),frameSize);
    }
    else if(strncmp(header.frameId,"SYLT",4)==0){
        STORE_LIST(SYLTFrameList,ID3v2DefaultFrameType,FramesV2_storeDefaultFrame);
        printf("NOT TESTED TAG %s: %ld\nSize: %d\n", header.frameId,ftell(mp3FilePointer),frameSize);
    }
    else if(strncmp(header.frameId,"GEOB",4)==0){
        STORE_LIST(GEOBFrameList,ID3v2DefaultFrameType,FramesV2_storeDefaultFrame);
        printf("NOT TESTED TAG %s: %ld\nSize: %d\n", header.frameId,ftell(mp3FilePointer),frameSize);
    }
    else if(strncmp(header.frameId,"LINK",4)==0){
        STORE_LIST(LINKFrameList,ID3v2DefaultFrameType,FramesV2_storeDefaultFrame);
        printf("NOT TESTED TAG %s: %ld\nSize: %d\n", header.frameId,ftell(mp3FilePointer),frameSize);
    }
    else if(strncmp(header.frameId,"AENC",4)==0){
        STORE_LIST(AENCFrameList,ID3v2DefaultFrameType,FramesV2_storeDefaultFrame);
        printf("NOT TESTED TAG %s: %ld\nSize: %d\n", header.frameId,ftell(mp3FilePointer),frameSize);
    }
    else if(strncmp(header.frameId,"ENCR",4)==0){
        STORE_LIST(ENCRFrameList,ID3v2DefaultFrameType,FramesV2_storeDefaultFrame);
        printf("NOT TESTED TAG %s: %ld\nSize: %d\n", header.frameId,ftell(mp3FilePointer),frameSize);
    }
    else if(strncmp(header.frameId,"GRID",4)==0){
        STORE_LIST(GRIDFrameList,ID3v2DefaultFrameType,FramesV2_storeDefaultFrame);
        printf("NOT TESTED TAG %s: %ld\nSize: %d\n", header.frameId,ftell(mp3FilePointer),frameSize);
    }
    else{
        printf("NOT SUPPORTED TAG %s: %ld\nSize: %d\n", header.frameId,ftell(mp3FilePointer),frameSize);
        uint8_t *buffer = (uint8_t *)malloc(frameSize);
        fread(buffer, frameSize, 1, mp3FilePointer);
        free(buffer);
    }
    return 0;
}