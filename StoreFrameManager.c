#include "StoreFrameManager.h"
#include <string.h>

bool StoreFrame_storeNextFrameInStruct(FILE *mp3FilePointer, ID3TagType *ID3Tag){
    ID3v2FrameHeaderType header;
    FramesV2_storeHeader(mp3FilePointer,&header);
    uint32_t frameSize = FramesV2_getFrameSize(ID3Tag->header.version[0],header); 
    printf("FrameID: %s\n",header.frameId);
    if (memcmp(header.frameId, "\0\0\0\0", 4) == 0) return true;
    else if(strncmp(header.frameId,"T",1)==0){
        ID3v2TXTFrameType *TXTF = (ID3v2TXTFrameType *)malloc(sizeof(ID3v2TXTFrameType));
        TXTF->header = header;
        FramesV2_storeTXTF(mp3FilePointer,frameSize, TXTF);
        ListFramePtr_insertLast(&ID3Tag->TXTFrameList,TXTF);
    }
    else if(strncmp(header.frameId,"COMM",4)==0){
        ID3v2COMMFrameType *COMM = (ID3v2COMMFrameType *)malloc(sizeof(ID3v2COMMFrameType));
        COMM->header = header;
        FramesV2_storeCOMM(mp3FilePointer,frameSize, COMM);
        ListFramePtr_insertLast(&ID3Tag->COMMFrameList,COMM);
    }
    else if(strncmp(header.frameId,"PRIV",4)==0){
        ID3v2PRIVFrameType *PRIV = (ID3v2PRIVFrameType *)malloc(sizeof(ID3v2PRIVFrameType));
        PRIV->header = header;
        FramesV2_storePRIV(mp3FilePointer,frameSize, PRIV);
        ListFramePtr_insertLast(&ID3Tag->PRIVFrameList,PRIV);
    }
    else if(strncmp(header.frameId,"APIC",4)==0){
        ID3v2APICFrameType *APIC = (ID3v2APICFrameType *)malloc(sizeof(ID3v2APICFrameType));;
        APIC->header = header;
        FramesV2_storeAPIC(mp3FilePointer,frameSize, APIC);
        ListFramePtr_insertLast(&ID3Tag->APICFrameList,APIC);
    }
    else if(strncmp(header.frameId,"POPM",4)==0){
        ID3v2POPMFrameType *POPM = (ID3v2POPMFrameType *)malloc(sizeof(ID3v2POPMFrameType));
        POPM->header = header;
        FramesV2_storePOPM(mp3FilePointer,frameSize, POPM);
        ListFramePtr_insertLast(&ID3Tag->POPMFrameList,POPM);
    }
    else if(strncmp(header.frameId,"WXXX",4)==0){
        printf("NOT TESTED TAG %s: %ld\nSize: %d\n", header.frameId,ftell(mp3FilePointer),frameSize);
        ID3v2WXXXFrameType *WXXX = (ID3v2WXXXFrameType *) malloc(sizeof(ID3v2WXXXFrameType));
        WXXX->header = header;
        FramesV2_storeWXXX(mp3FilePointer,frameSize,WXXX);
        ListFramePtr_insertLast(&ID3Tag->WXXXFrameList,WXXX);
    }
    else if(strncmp(header.frameId,"W",1)==0){
        printf("NOT TESTED TAG %s: %ld\nSize: %d\n", header.frameId,ftell(mp3FilePointer),frameSize);
        ID3v2WWWFrameType *WWWF = (ID3v2WWWFrameType *) malloc(sizeof(ID3v2WWWFrameType));
        WWWF->header = header;
        FramesV2_storeWWWF(mp3FilePointer,frameSize,WWWF);
        ListFramePtr_insertLast(&ID3Tag->WWWFrameList,WWWF);
    }
    else if(strncmp(header.frameId,"MCDI",4)==0){
        FramesV2_storeMDCI(mp3FilePointer,frameSize, &ID3Tag->MCDI);
        ID3Tag->MCDI->header = header;
    }
    //DEFAULT FRAMES
    else if(strncmp(header.frameId,"IPLS",4)==0){
        printf("NOT TESTED TAG %s: %ld\nSize: %d\n", header.frameId,ftell(mp3FilePointer),frameSize);
        FramesV2_storeDefaultFrame(mp3FilePointer,frameSize,&ID3Tag->IPLS);
        ID3Tag->IPLS->header = header;
    }
    else if(strncmp(header.frameId,"SYTC",4)==0){
        printf("NOT TESTED TAG %s: %ld\nSize: %d\n", header.frameId,ftell(mp3FilePointer),frameSize);
        FramesV2_storeDefaultFrame(mp3FilePointer,frameSize,&ID3Tag->SYTC);
        ID3Tag->SYTC->header = header;
    }
    else if(strncmp(header.frameId,"USER",4)==0){
        printf("NOT TESTED TAG %s: %ld\nSize: %d\n", header.frameId,ftell(mp3FilePointer),frameSize);
        FramesV2_storeDefaultFrame(mp3FilePointer,frameSize,&ID3Tag->USER);
        ID3Tag->USER->header = header;
    }
    else if(strncmp(header.frameId,"OWNE",4)==0){
        printf("NOT TESTED TAG %s: %ld\nSize: %d\n", header.frameId,ftell(mp3FilePointer),frameSize);
        FramesV2_storeDefaultFrame(mp3FilePointer,frameSize,&ID3Tag->OWNE);
        ID3Tag->OWNE->header = header;
    }
    else if(strncmp(header.frameId,"PCNT",4)==0){
        printf("NOT TESTED TAG %s: %ld\nSize: %d\n", header.frameId,ftell(mp3FilePointer),frameSize);
        FramesV2_storeDefaultFrame(mp3FilePointer,frameSize,&ID3Tag->PCNT);
        ID3Tag->PCNT->header = header;
    }
    else if(strncmp(header.frameId,"RVRB",4)==0){
        printf("NOT TESTED TAG %s: %ld\nSize: %d\n", header.frameId,ftell(mp3FilePointer),frameSize);
        FramesV2_storeDefaultFrame(mp3FilePointer,frameSize,&ID3Tag->RVRB);
        ID3Tag->RVRB->header = header;
    }
    else if(strncmp(header.frameId,"EQUA",4)==0){
        printf("NOT TESTED TAG %s: %ld\nSize: %d\n", header.frameId,ftell(mp3FilePointer),frameSize);
        FramesV2_storeDefaultFrame(mp3FilePointer,frameSize,&ID3Tag->EQUA);
        ID3Tag->EQUA->header = header;
    }
    else if(strncmp(header.frameId,"MLLT",4)==0){
        printf("NOT TESTED TAG %s: %ld\nSize: %d\n", header.frameId,ftell(mp3FilePointer),frameSize);
        FramesV2_storeDefaultFrame(mp3FilePointer,frameSize,&ID3Tag->MLLT);
        ID3Tag->MLLT->header = header;
    }
    else if(strncmp(header.frameId,"ETCO",4)==0){
        printf("NOT TESTED TAG %s: %ld\nSize: %d\n", header.frameId,ftell(mp3FilePointer),frameSize);
        FramesV2_storeDefaultFrame(mp3FilePointer,frameSize,&ID3Tag->ETCO);
        ID3Tag->ETCO->header = header;
    }
    else if(strncmp(header.frameId,"RVAD",4)==0){
        printf("NOT TESTED TAG %s: %ld\nSize: %d\n", header.frameId,ftell(mp3FilePointer),frameSize);
        FramesV2_storeDefaultFrame(mp3FilePointer,frameSize,&ID3Tag->RVAD);
        ID3Tag->RVAD->header = header;
    }
    else if(strncmp(header.frameId,"POSS",4)==0){
        printf("NOT TESTED TAG %s: %ld\nSize: %d\n", header.frameId,ftell(mp3FilePointer),frameSize);
        FramesV2_storeDefaultFrame(mp3FilePointer,frameSize,&ID3Tag->POSS);
        ID3Tag->COMR->header = header;
    }
    else if(strncmp(header.frameId,"COMR",4)==0){
        printf("NOT TESTED TAG %s: %ld\nSize: %d\n", header.frameId,ftell(mp3FilePointer),frameSize);
        FramesV2_storeDefaultFrame(mp3FilePointer,frameSize,&ID3Tag->COMR);
        ID3Tag->COMR->header = header;
    }
    //default Lists
    else if(strncmp(header.frameId,"UFID",4)==0){
        printf("NOT TESTED TAG %s: %ld\nSize: %d\n", header.frameId,ftell(mp3FilePointer),frameSize);
        ID3v2DefaultFrameType *framePtr =(ID3v2DefaultFrameType *)malloc(sizeof(ID3v2DefaultFrameType));  
        framePtr->header = header;
        FramesV2_storeDefaultFrame(mp3FilePointer,frameSize,&framePtr);
        ListFramePtr_insertLast(&ID3Tag->UFIDFrameList,framePtr);
    }
    else if(strncmp(header.frameId,"USLT",4)==0){
        printf("NOT TESTED TAG %s: %ld\nSize: %d\n", header.frameId,ftell(mp3FilePointer),frameSize);
        ID3v2DefaultFrameType *framePtr =(ID3v2DefaultFrameType *)malloc(sizeof(ID3v2DefaultFrameType));  
        framePtr->header = header;
        FramesV2_storeDefaultFrame(mp3FilePointer,frameSize,&framePtr);
        ListFramePtr_insertLast(&ID3Tag->USLTFrameList,framePtr);
    }
    else if(strncmp(header.frameId,"SYLT",4)==0){
        printf("NOT TESTED TAG %s: %ld\nSize: %d\n", header.frameId,ftell(mp3FilePointer),frameSize);
        ID3v2DefaultFrameType *framePtr =(ID3v2DefaultFrameType *)malloc(sizeof(ID3v2DefaultFrameType));  
        framePtr->header = header;
        FramesV2_storeDefaultFrame(mp3FilePointer,frameSize,&framePtr);
        ListFramePtr_insertLast(&ID3Tag->SYLTFrameList,framePtr);
    }
    else if(strncmp(header.frameId,"GEOB",4)==0){
        printf("NOT TESTED TAG %s: %ld\nSize: %d\n", header.frameId,ftell(mp3FilePointer),frameSize);
        ID3v2DefaultFrameType *framePtr =(ID3v2DefaultFrameType *)malloc(sizeof(ID3v2DefaultFrameType));  
        framePtr->header = header;
        FramesV2_storeDefaultFrame(mp3FilePointer,frameSize,&framePtr);
        ListFramePtr_insertLast(&ID3Tag->GEOBFrameList,framePtr);
    }
    else if(strncmp(header.frameId,"LINK",4)==0){
        printf("NOT TESTED TAG %s: %ld\nSize: %d\n", header.frameId,ftell(mp3FilePointer),frameSize);
        ID3v2DefaultFrameType *framePtr =(ID3v2DefaultFrameType *)malloc(sizeof(ID3v2DefaultFrameType));  
        framePtr->header = header;
        FramesV2_storeDefaultFrame(mp3FilePointer,frameSize,&framePtr);
        ListFramePtr_insertLast(&ID3Tag->LINKFrameList,framePtr);
    }
    else if(strncmp(header.frameId,"AENC",4)==0){
        printf("NOT TESTED TAG %s: %ld\nSize: %d\n", header.frameId,ftell(mp3FilePointer),frameSize);
        ID3v2DefaultFrameType *framePtr =(ID3v2DefaultFrameType *)malloc(sizeof(ID3v2DefaultFrameType));  
        framePtr->header = header;
        FramesV2_storeDefaultFrame(mp3FilePointer,frameSize,&framePtr);
        ListFramePtr_insertLast(&ID3Tag->AENCFrameList,framePtr);
    }
    else if(strncmp(header.frameId,"ENCR",4)==0){
        printf("NOT TESTED TAG %s: %ld\nSize: %d\n", header.frameId,ftell(mp3FilePointer),frameSize);
        ID3v2DefaultFrameType *framePtr =(ID3v2DefaultFrameType *)malloc(sizeof(ID3v2DefaultFrameType));  
        framePtr->header = header;
        FramesV2_storeDefaultFrame(mp3FilePointer,frameSize,&framePtr);
        ListFramePtr_insertLast(&ID3Tag->ENCRFrameList,framePtr);
    }
    else if(strncmp(header.frameId,"GRID",4)==0){
        printf("NOT TESTED TAG %s: %ld\nSize: %d\n", header.frameId,ftell(mp3FilePointer),frameSize);
        ID3v2DefaultFrameType *framePtr =(ID3v2DefaultFrameType *)malloc(sizeof(ID3v2DefaultFrameType));  
        framePtr->header = header;
        FramesV2_storeDefaultFrame(mp3FilePointer,frameSize,&framePtr);
        ListFramePtr_insertLast(&ID3Tag->GRIDFrameList,framePtr);
    }
    else{
        printf("NOT SUPPORTED TAG %s: %ld\nSize: %d\n", header.frameId,ftell(mp3FilePointer),frameSize);
        uint8_t *buffer = (uint8_t *)malloc(frameSize);
        fread(buffer, frameSize, 1, mp3FilePointer);
        free(buffer);
    }
    return 0;
}