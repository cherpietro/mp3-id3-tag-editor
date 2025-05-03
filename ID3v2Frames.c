#include "ID3v2Frames.h"
#include "SizeReader.h"
#include <string.h>

uint32_t FramesV2_getFrameSize(int version, ID3v2FrameHeaderType header){
    if(version == 4) return syncsafeToSize(header.size);
    else return sizeFromID3v23(header.size);
}
void FramesV2_updateFrameSize(int version, ID3v2FrameHeaderType *header,uint32_t newSize){
    if(version == 4) saveSizeToSyncsafe(newSize,header->size);
    else saveSizeToID3v23(newSize,header->size);
}

void FramesV2_freeAPIC(ID3v2APICFrameType** APIC){
    TxtStr_freeTextString(&(*APIC)->mimeType);
    TxtStr_freeTextString(&(*APIC)->description);
    free((*APIC)->imageData);
    free(*APIC);
    *APIC = NULL;
}
ID3v2APICFrameType *GetFrame_APIC(int version){
    ID3v2APICFrameType *APICFramePtr = (ID3v2APICFrameType *) malloc(sizeof(ID3v2APICFrameType));
    char description[65]; //max length is 64 characters
    memcpy(APICFramePtr->header.frameId,"APIC",4); 
    printf("Introduce description of the image (can be empty|max 64 characters): ");
    fgets(description, sizeof(description), stdin);
    description[strcspn(description, "\n")] = 0;
    TxtStr_storeTextString(&APICFramePtr->description,description,strlen(description)+1);
    TxtStr_storeTextString(&APICFramePtr->mimeType,"image/jpeg\0",11); 
    APICFramePtr->textEncoding = 3;
    APICFramePtr->pictureType = 0;
    
    char coverFileName[35];
    printf("Introduce the name of the image: ");
    fgets(coverFileName, sizeof(coverFileName), stdin);
    coverFileName[strcspn(coverFileName, "\n")] = 0;
    
    FILE *coverPtr = fopen(coverFileName, "rb");
    // FILE *coverPtr = fopen("pipoCover.jpg", "rb");

    if (coverPtr){        
        fseek(coverPtr, 0, SEEK_END);
        APICFramePtr->imageDataSize = ftell(coverPtr); 
        fseek(coverPtr, 0, SEEK_SET);
        APICFramePtr->imageData = (uint8_t *)malloc(APICFramePtr->imageDataSize);
        size_t bytesRead = fread(APICFramePtr->imageData, 1, APICFramePtr   ->imageDataSize, coverPtr);
        if (bytesRead != APICFramePtr->imageDataSize) {
            free(APICFramePtr->imageData);
        }    
        fclose(coverPtr);
        uint32_t newSize = APICFramePtr->description.size + APICFramePtr->imageDataSize + APICFramePtr->mimeType.size;
        FramesV2_updateFrameSize(version,&APICFramePtr->header,newSize+2);
        return APICFramePtr;
    }
    else{
        printf("Error opening cover file\n");
        FramesV2_freeAPIC(&APICFramePtr);
        return NULL;
    }
}
ID3v2TXTFrameType * GetFrame_TXXX(int version){
    char description[65];
    char value[255];
    char *mergedString;

    ID3v2TXTFrameType *TXTFramePtr = (ID3v2TXTFrameType*) malloc(sizeof(ID3v2TXTFrameType));
    memcpy(TXTFramePtr->header.frameId,"TXXX",4); 
    TXTFramePtr->header.flags[0] = 0;TXTFramePtr->header.flags[1] = 0;
    TXTFramePtr->textEncoding = 3;
    printf("Insert tag description (max. size 64): ");
    fgets(description, sizeof(description), stdin);
    description[strcspn(description, "\n")] = 0;
    printf("\n");
    printf("Insert tag content (max. size 254): ");
    fgets(value, sizeof(value), stdin);
    printf("\n");
    value[strcspn(value, "\n")] = 0;
    int totalLen = strlen(value)+strlen(description)+2;
    mergedString = (char *) malloc(totalLen);
    memcpy(mergedString, description, strlen(description)+1);
    memcpy(mergedString + strlen(description)+1, value, strlen(value)+1);
    TxtStr_storeTextString(&TXTFramePtr->content,mergedString,totalLen);
    free(mergedString);
    FramesV2_updateFrameSize(version,&TXTFramePtr->header,TXTFramePtr->content.size+1);
    return TXTFramePtr;
}

void FramesV2_freeTXTF(ID3v2TXTFrameType** TXTF){
    TxtStr_freeTextString(&(*TXTF)->content);
    free(*TXTF);
    *TXTF = NULL;
}
void FramesV2_freeWXXX(ID3v2WXXXFrameType **WXXX){
    TxtStr_freeTextString(&(*WXXX)->description);
    TxtStr_freeTextString(&(*WXXX)->url);
    free(*WXXX);
    *WXXX = NULL;
}
void FramesV2_freeCOMM(ID3v2COMMFrameType** COMM){
    TxtStr_freeTextString(&(*COMM)->actualText);
    TxtStr_freeTextString(&(*COMM)->contentDescript);
    free(*COMM);
    *COMM = NULL;
}
void FramesV2_freePRIV(ID3v2PRIVFrameType** PRIV){
    TxtStr_freeTextString(&(*PRIV)->owner);
    TxtStr_freeTextString(&(*PRIV)->privateData);
    free(*PRIV);
    *PRIV = NULL;
}
void FramesV2_freeMCDI(ID3v2MCDIFrameType **MCDI){
    TxtStr_freeTextString(&(*MCDI)->CDTOC);
    free(*MCDI);
    *MCDI = NULL;
}
void FramesV2_freePOPM(ID3v2POPMFrameType **POPM){
    TxtStr_freeTextString(&(*POPM)->userEmail);
    free(*POPM);
    *POPM = NULL;
}
void FramesV2_freeWWWF(ID3v2WWWFrameType **WWWF){
    TxtStr_freeTextString(&(*WWWF)->url);
    free(*WWWF);
    *WWWF = NULL;
}
void FramesV2_freeDefaultFrame(ID3v2DefaultFrameType **DefaultFrame){
    free((*DefaultFrame)->frameData);
    free(*DefaultFrame);
    *DefaultFrame = NULL;
}
