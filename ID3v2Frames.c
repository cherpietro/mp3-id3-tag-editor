#include "ID3v2Frames.h"
#include "SizeReader.h"
#include <string.h>
#include <ctype.h>

void FramesV2_storeHeader(FILE *mp3FilePointer, ID3v2FrameHeaderType *header){
    fread(header, sizeof(ID3v2FrameHeaderType), 1, mp3FilePointer);
}
uint32_t FramesV2_getFrameSize(int version, ID3v2FrameHeaderType header){
    if(version == 4) return syncsafeToSize(header.size);
    else return sizeFromID3v23(header.size);
}
void FramesV2_updateFrameSize(int version, ID3v2FrameHeaderType *header,uint32_t newSize){
    if(version == 4) saveSizeToSyncsafe(newSize,header->size);
    else saveSizeToID3v23(newSize,header->size);
}

void FramesV2_storeAPIC(FILE *mp3FilePointer, uint32_t frameSize,ID3v2APICFrameType* apic){
    uint8_t *frameContent = (uint8_t *)malloc(frameSize);
    fread(frameContent, frameSize, 1, mp3FilePointer);

    // *apic = (ID3v2APICFrameType *)malloc(sizeof(ID3v2APICFrameType));
    
    size_t index = 0;
    (apic)->textEncoding = frameContent[index++];

    char *mimeType = (char*) frameContent + index;
    size_t mimeTypeLen = strlen(mimeType)+1;
    TxtStr_storeTextString(&(apic)->mimeType,mimeType,mimeTypeLen);
    index+=mimeTypeLen;
    
    (apic)->pictureType = frameContent[index++];

    char *description = (char*) frameContent + index;
    size_t descriptionLen = strlen(description)+1;
    TxtStr_storeTextString(&(apic)->description,description,descriptionLen);
    index += descriptionLen;;

    (apic)->imageDataSize = frameSize - index;
    (apic)->imageData = (uint8_t *)malloc((apic)->imageDataSize);
    memcpy((apic)->imageData, frameContent + index, (apic)->imageDataSize);
    free(frameContent);
}
void FramesV2_saveAPICImage(ID3v2APICFrameType frame){
    FILE *imageFile = fopen("cover.jpg", "wb");
    fwrite(frame.imageData, 1, frame.imageDataSize, imageFile);
    fclose(imageFile);
    printf("File saved in ./cover.jpg\n");
}
void FramesV2_printAPIC(ID3v2APICFrameType frame){
    printf("\n----FRAME----\n");
    printf("Frame ID: %s\n",frame.header.frameId);
    printf("Flags: %u %u\n",frame.header.flags[0],frame.header.flags[1]);
    printf("TextEncoding: %d\n",frame.textEncoding);
    printf("MIME type: %s\n",frame.mimeType.string);
    printf("apicframe.pictureType: %u\n",frame.pictureType);
    printf("apicframe.description: %s\n",frame.description.string);
    printf("apicframe.imageSize: %ld\n",frame.imageDataSize);
    FramesV2_saveAPICImage(frame);
}
void FramesV2_freeAPIC(ID3v2APICFrameType** APIC){
    TxtStr_freeTextString(&(*APIC)->mimeType);
    TxtStr_freeTextString(&(*APIC)->description);
    free((*APIC)->imageData);
    free(*APIC);
    *APIC = NULL;
}
void FramesV2_ModifyAPIC(uint8_t version,ID3v2APICFrameType *APIC){
    int c;
    // FILE *newCover = fopen("overtureCover.jpg", "rb");
    
    char description[65]; //max length is 64 characters
    printf("Introduce description of the image (can be empty|max 64 characters): ");
    fgets(description, sizeof(description), stdin);
    description[strcspn(description, "\n")] = 0;

    TxtStr_freeTextString(&APIC->description);
    TxtStr_storeTextString(&APIC->description,description,strlen(description)+1);
    TxtStr_storeTextString(&APIC->description,description,strlen(description)+1);

    char newCoverFileName[35];
    printf("Introduce the name of the image: ");
    scanf("%34s", newCoverFileName);
    while ((c = getchar()) != '\n' && c != EOF);
    FILE *newCover = fopen(newCoverFileName, "rb");
    // FILE *newCover = fopen("pipoCover.jpg", "rb");
    if (newCover) {
        free(APIC->imageData);
        
        fseek(newCover, 0, SEEK_END);
        APIC->imageDataSize = ftell(newCover); 
        fseek(newCover, 0, SEEK_SET);
        
        APIC->imageData = (uint8_t *)malloc(APIC->imageDataSize);
        
        size_t bytesRead = fread(APIC->imageData, 1, APIC->imageDataSize, newCover);
        if (bytesRead != APIC->imageDataSize) {
            free(APIC->imageData);
        }    
        fclose(newCover);
        uint32_t newSize = APIC->description.size + APIC->imageDataSize;
        printf("OLD SIZE: %d\n",FramesV2_getFrameSize(version,APIC->header));
        FramesV2_updateFrameSize(version,&APIC->header,newSize);
        printf("NEW SIZE: %d\n",FramesV2_getFrameSize(version,APIC->header));
    }
}

void FramesV2_storeTXTF(FILE *mp3FilePointer, uint32_t frameSize, ID3v2TXTFrameType *frame){
    uint8_t *frameContent = (uint8_t *)malloc(frameSize);
    fread(frameContent, frameSize, 1, mp3FilePointer);

    frame->textEncoding = frameContent[0];
    char *contentPtr = (char *)frameContent + 1;
    TxtStr_storeTextString(&frame->content,contentPtr, frameSize-1);
    free(frameContent);
}
ID3v2TXTFrameType * FramesV2_getTXXX(){
    char description[65];
    char value[255];
    char *mergedString;

    ID3v2TXTFrameType *TXTFramePtr = (ID3v2TXTFrameType*) malloc(sizeof(ID3v2TXTFrameType));
    memcpy(TXTFramePtr->header.frameId,"TXXX",4); 
    TXTFramePtr->header.flags[0] = 0;TXTFramePtr->header.flags[1] = 0;
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
    return TXTFramePtr;
}
ID3v2TXTFrameType * FramesV2_getTXTF(char * frameID){
    char content[255];

    ID3v2TXTFrameType *TXTFramePtr = (ID3v2TXTFrameType*) malloc(sizeof(ID3v2TXTFrameType));
    for (int i = 0; i < 4; i++) frameID[i] = toupper(frameID[i]);
    memcpy(TXTFramePtr->header.frameId,frameID,4); 
    TXTFramePtr->header.flags[0] = 0;TXTFramePtr->header.flags[1] = 0;
    TXTFramePtr->textEncoding = 0;
    printf("Insert tag content (max. size 254): ");
    fgets(content, sizeof(content), stdin);
    content[strcspn(content, "\n")] = 0;
    printf("\n");
    TxtStr_storeTextString(&TXTFramePtr->content,content,strlen(content)+1);
    return TXTFramePtr;
}
void FramesV2_printTXTF(ID3v2TXTFrameType frame){
    printf("\n----FRAME----\n");
    printf("Frame ID: %s\n",frame.header.frameId);
    printf("Flags: %u %u\n",frame.header.flags[0],frame.header.flags[1]);
    printf("TextEncoding: %d\n",frame.textEncoding);
    if(strncasecmp("TXXX",frame.header.frameId,4) == 0){
        printf("Description: ");
        for (int i = 0; i < (int)strlen(frame.content.string); i++) {
            if(frame.content.string[i] == '\0') printf(" ");
            else putchar(frame.content.string[i]);
        }
        printf("\n");
        printf("Content: ");
        for (int i = strlen(frame.content.string)+1; i < (int)frame.content.size; i++) {
            if(frame.content.string[i] == '\0') printf(" ");
            else putchar(frame.content.string[i]);
        }
        printf("\n");
    }
    else{
        printf("Content: ");
        for (int i = 0; i < (int)frame.content.size; i++) {
            if(frame.content.string[i] == '\0') printf(" ");
            else putchar(frame.content.string[i]);
        }
        printf("\n");
    }

}
void FramesV2_freeTXTF(ID3v2TXTFrameType** TXTF){
    TxtStr_freeTextString(&(*TXTF)->content);
    free(*TXTF);
    *TXTF = NULL;
}
bool FramesV2_validTextFrameId(char *str) {
    const char *frames[] = {
        "TALB", "TBPM", "TCOM", "TCON", "TCOP", "TDAT", "TDLY", "TENC", "TEXT", "TFLT", 
        "TIME", "TIT1", "TIT2", "TIT3", "TKEY", "TLAN", "TLEN", "TMED", "TOAL", "TOFN", 
        "TOLY", "TOPE", "TORY", "TOWN", "TPE1", "TPE2", "TPE3", "TPE4", "TPOS", "TPUB", 
        "TRCK", "TRDA", "TRSN", "TRSO", "TSIZ", "TSRC", "TSSE", "TYER"
    };
    for (int i = 0; i < 34; i++) {
        if (strncasecmp(str, frames[i],4) == 0) return true;  
    }
    return false;
}
void FramesV2_ModifyTXTF(uint8_t version,ID3v2TXTFrameType *TXTF){
    char content[256];
    printf("Introduce the new text modify: ");
    fgets(content, sizeof(content), stdin);
    content[strcspn(content, "\n")] = 0;
    
    printf("%s\n",content);
    TxtStr_freeTextString(&TXTF->content);
    TxtStr_storeTextString(&TXTF->content,content,strlen(content)+1);

    uint32_t newSize = TXTF->content.size + 1;
    printf("OLD SIZE: %d\n",FramesV2_getFrameSize(version,TXTF->header));
    FramesV2_updateFrameSize(version,&TXTF->header,newSize);
    printf("NEW SIZE: %d\n",FramesV2_getFrameSize(version,TXTF->header));
}

void FramesV2_storeCOMM(FILE *mp3FilePointer, uint32_t frameSize, ID3v2COMMFrameType *frame){
    uint8_t *frameContent = (uint8_t *)malloc(frameSize);
    fread(frameContent, frameSize, 1, mp3FilePointer);

    size_t index = 0;
    frame->textEncoding = frameContent[index++];
    frame->language[0] = frameContent[index];
    frame->language[1] = frameContent[index+1];
    frame->language[2] = frameContent[index+2];
    index+=3;

    char *descPtr = (char *)frameContent + index;
    size_t descSize = strlen(descPtr)+1; //description ALWAYS has to have \0
    TxtStr_storeTextString(&frame->contentDescript,descPtr, descSize);
    index += descSize;

    char *contentPtr = (char *)frameContent + index ; 
    size_t contentSize = frameSize - index ; //Check this operation
    TxtStr_storeTextString(&frame->actualText,contentPtr, contentSize);
    free(frameContent);
}
void FramesV2_printCOMM(ID3v2COMMFrameType frame){
    printf("\n----FRAME----\n");
    printf("Frame ID: %s\n",frame.header.frameId);
    printf("Flags: %u %u\n",frame.header.flags[0],frame.header.flags[1]);
    printf("TextEncoding: %d\n",frame.textEncoding);
    printf("Language: %s\n",frame.language);
    printf("Descript: ");
    for (int i = 0; i < (int)frame.contentDescript.size; i++) {
        if(frame.contentDescript.string[i] == '\0') printf(" ");
        else putchar(frame.contentDescript.string[i]);
    }
    printf("\n");
    printf("Text: ");
    for (int i = 0; i < (int)frame.actualText.size; i++) {
        if(frame.actualText.string[i] == '\0') printf(" ");
        else putchar(frame.actualText.string[i]);
    }
    printf("\n");
    
}
void FramesV2_freeCOMM(ID3v2COMMFrameType** COMM){
    TxtStr_freeTextString(&(*COMM)->actualText);
    TxtStr_freeTextString(&(*COMM)->contentDescript);
    free(*COMM);
    *COMM = NULL;
}

void FramesV2_printPRIV(ID3v2PRIVFrameType frame){
    printf("\n----FRAME----\n");
    printf("Frame ID: %s\n",frame.header.frameId);
    printf("Flags: %u %u\n",frame.header.flags[0],frame.header.flags[1]);
    printf("Owner: ");
    for (int i = 0; i < (int)frame.owner.size; i++) {
        if(frame.owner.string[i] == '\0') printf(" ");
        else putchar(frame.owner.string[i]);
    }
    printf("\n"); 
}
void FramesV2_storePRIV(FILE *mp3FilePointer, uint32_t frameSize, ID3v2PRIVFrameType *frame){
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
void FramesV2_freePRIV(ID3v2PRIVFrameType** PRIV){
    TxtStr_freeTextString(&(*PRIV)->owner);
    TxtStr_freeTextString(&(*PRIV)->privateData);
    free(*PRIV);
    *PRIV = NULL;
}

void FramesV2_storeMDCI(FILE *mp3FilePointer, uint32_t frameSize,ID3v2MCDIFrameType** MDCI){
    *MDCI = (ID3v2MCDIFrameType *)malloc(sizeof(ID3v2MCDIFrameType));
    uint8_t *frameContent = (uint8_t *)malloc(frameSize);
    fread(frameContent, frameSize, 1, mp3FilePointer);
    char *ptr = (char *)frameContent;
    TxtStr_storeTextString(&(*MDCI)->CDTOC,ptr, frameSize);
    free(frameContent);
}
void FramesV2_printMDCI(ID3v2MCDIFrameType MDCI){
    printf("\n----FRAME----\n");
    printf("Frame ID: %s\n",MDCI.header.frameId);
    printf("Flags: %u %u\n",MDCI.header.flags[0],MDCI.header.flags[1]);
}
void FramesV2_freeMCDI(ID3v2MCDIFrameType **MCDI){
    TxtStr_freeTextString(&(*MCDI)->CDTOC);
    free(*MCDI);
    *MCDI = NULL;
}

void FramesV2_storePOPM(FILE *mp3FilePointer, uint32_t frameSize,ID3v2POPMFrameType* POPM){
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
    (POPM)->counter[0] = counterPtr[0];
    (POPM)->counter[1] = counterPtr[1];
    (POPM)->counter[2] = counterPtr[2];
    (POPM)->counter[3] = counterPtr[3];
    free(frameContent);
}
void FramesV2_printPOPM(ID3v2POPMFrameType POPM){
    printf("\n----FRAME----\n");
    printf("Frame ID: %s\n",POPM.header.frameId);
    printf("Flags: %u %u\n",POPM.header.flags[0],POPM.header.flags[1]);
    printf("User email: %s\n",POPM.userEmail.string);
    printf("Ratin: %d\n",POPM.rating);
}
void FramesV2_freePOPM(ID3v2POPMFrameType **POPM){
    TxtStr_freeTextString(&(*POPM)->userEmail);
    free(*POPM);
    *POPM = NULL;
}

void FramesV2_storeIPLS(FILE* mp3FilePointer, uint32_t frameSize, ID3v2IPLSFrameType **IPLS){
    *IPLS = (ID3v2IPLSFrameType *)malloc(sizeof(ID3v2IPLSFrameType));
    uint8_t *frameContent = (uint8_t *)malloc(frameSize);
    fread(frameContent, frameSize, 1, mp3FilePointer);

    (*IPLS)->textEncoding = frameContent[0];
    char *peopleLstPtr = (char *)frameContent + 1;
    TxtStr_storeTextString(&(*IPLS)->peopeList,peopleLstPtr,frameSize -1);
    free(frameContent);
}
void FramesV2_printIPLS(ID3v2IPLSFrameType IPLS){
    printf("\n----FRAME----\n");
    printf("Frame ID: %s\n",IPLS.header.frameId);
    printf("Flags: %u %u\n",IPLS.header.flags[0],IPLS.header.flags[1]);
    printf("TextEncoding: %d\n",IPLS.textEncoding);
    printf("People List: ");
    for (int i = 0; i < (int)IPLS.peopeList.size; i++) {
        if(IPLS.peopeList.string[i] == '\0') printf("\n");
        else putchar(IPLS.peopeList.string[i]);
    }
    printf("\n");
}
void FramesV2_freeIPLS(ID3v2IPLSFrameType **IPLS){
    TxtStr_freeTextString(&(*IPLS)->peopeList);
    free(*IPLS);
    *IPLS = NULL;
}

void FramesV2_storeSYTC(FILE* mp3FilePointer, uint32_t frameSize, ID3v2SYTCFrameType **SYTC){
    *SYTC = (ID3v2SYTCFrameType *)malloc(sizeof(ID3v2SYTCFrameType));
    uint8_t *frameContent = (uint8_t *)malloc(frameSize);
    fread(frameContent, frameSize, 1, mp3FilePointer);

    (*SYTC)->timestampFormat = frameContent[0];
    uint8_t *tempoDataPtr = (uint8_t *)frameContent + 1;
    (*SYTC)->tempoData =    (uint8_t *)malloc(frameSize-1);
    memcpy((*SYTC)->tempoData,tempoDataPtr,frameSize-1);
    free(frameContent);
}
void FramesV2_printSYTC(ID3v2SYTCFrameType SYTC){
    printf("\n----FRAME----\n");
    printf("Frame ID: %s\n",SYTC.header.frameId);
    printf("Flags: %u %u\n",SYTC.header.flags[0],SYTC.header.flags[1]);
    printf("Timestamp format: %d\n",SYTC.timestampFormat);
}
void FramesV2_freeSYTC(ID3v2SYTCFrameType **SYTC){
    free((*SYTC)->tempoData);
    free(*SYTC);
    *SYTC = NULL;
}

void FramesV2_storeUSER(FILE* mp3FilePointer, uint32_t frameSize, ID3v2USERFrameType **USER){
    *USER = (ID3v2USERFrameType *)malloc(sizeof(ID3v2USERFrameType));
    uint8_t *frameContent = (uint8_t *)malloc(frameSize);
    fread(frameContent, frameSize, 1, mp3FilePointer);

    size_t index = 0;
    (*USER)->textEncoding = frameContent[index++];
    (*USER)->language[0] = frameContent[index];
    (*USER)->language[1] = frameContent[index+1];
    (*USER)->language[2] = frameContent[index+2];
    index+=3;

    char *textPtr = (char *)frameContent + index ; 
    size_t textSize = frameSize - index ; //Check this operation
    TxtStr_storeTextString(&(*USER)->actualText,textPtr, textSize);
    free(frameContent);
}
void FramesV2_printUSER(ID3v2USERFrameType USER){
    printf("\n----FRAME----\n");
    printf("Frame ID: %s\n",USER.header.frameId);
    printf("Flags: %u %u\n",USER.header.flags[0],USER.header.flags[1]);
    printf("Text Encoding: %d\n",USER.textEncoding);
    printf("Language: %s\n",USER.language);
    printf("Text: ");
    for (int i = 0; i < (int)USER.actualText.size; i++) {
        if(USER.actualText.string[i] == '\0') printf(" ");
        else putchar(USER.actualText.string[i]);
    }
    printf("\n");
}
void FramesV2_freeUSER(ID3v2USERFrameType **USER){
    TxtStr_freeTextString(&(*USER)->actualText);
    free(*USER);
    *USER = NULL;
}

void FramesV2_storeOWNE(FILE *mp3FilePointer, uint32_t frameSize, ID3v2OWNEFrameType **OWNE){
    *OWNE = (ID3v2OWNEFrameType *)malloc(sizeof(ID3v2OWNEFrameType));
    uint8_t *frameContent = (uint8_t *)malloc(frameSize);
    fread(frameContent, frameSize, 1, mp3FilePointer);

    size_t index = 0;
    (*OWNE)->textEncoding = frameContent[index++];
    char *prizePtr = (char *)frameContent + index;
    size_t prizeSize = strlen(prizePtr)+1; //prize ALWAYS has to have \0
    TxtStr_storeTextString(&(*OWNE)->pricePayed,prizePtr, prizeSize);
    index += prizeSize;
    
    (*OWNE)->dateOfPurch[0] = frameContent[index];
    (*OWNE)->dateOfPurch[1] = frameContent[index+1];
    (*OWNE)->dateOfPurch[2] = frameContent[index+2];
    (*OWNE)->dateOfPurch[3] = frameContent[index+3];
    (*OWNE)->dateOfPurch[4] = frameContent[index+4];
    (*OWNE)->dateOfPurch[5] = frameContent[index+5];
    (*OWNE)->dateOfPurch[6] = frameContent[index+6];
    (*OWNE)->dateOfPurch[7] = frameContent[index+7];
    index += 8;
    
    char *sellerPtr = (char *)frameContent + index ; 
    size_t sellerSize = frameSize - index ; //Check this operation
    TxtStr_storeTextString(&(*OWNE)->seller,sellerPtr, sellerSize);
    free(frameContent);
}
void FramesV2_printOWNE(ID3v2OWNEFrameType OWNE){
    printf("\n----FRAME----\n");
    printf("Frame ID: %s\n",OWNE.header.frameId);
    printf("Flags: %u %u\n",OWNE.header.flags[0],OWNE.header.flags[1]);
    printf("TextEncoding: %d\n",OWNE.textEncoding);
    printf("Price payed:");
    for (int i = 0; i < (int)OWNE.pricePayed.size; i++) {
        if(OWNE.pricePayed.string[i] == '\0') printf(" ");
        else putchar(OWNE.pricePayed.string[i]);
    }
    printf("\n");
    printf("Date of pruchase: %s\n",OWNE.dateOfPurch);
    printf("Seller: ");
    for (int i = 0; i < (int)OWNE.seller.size; i++) {
        if(OWNE.seller.string[i] == '\0') printf(" ");
        else putchar(OWNE.seller.string[i]);
    }
    printf("\n");
    
}
void FramesV2_freeOWNE(ID3v2OWNEFrameType** OWNE){
    TxtStr_freeTextString(&(*OWNE)->pricePayed);
    TxtStr_freeTextString(&(*OWNE)->seller);
    free(*OWNE);
    *OWNE = NULL;
}

void FramesV2_storePCNT(FILE* mp3FilePointer, uint32_t frameSize, ID3v2PCNTFrameType **PCNT){
    *PCNT = (ID3v2PCNTFrameType *)malloc(sizeof(ID3v2PCNTFrameType));
    uint8_t *frameContent = (uint8_t *)malloc(frameSize);
    fread(frameContent, frameSize, 1, mp3FilePointer);

    char *counterPtr = (char *)frameContent;
    TxtStr_storeTextString(&(*PCNT)->counter,counterPtr,frameSize);
    free(frameContent);
}
void FramesV2_printPCNT(ID3v2PCNTFrameType PCNT){
    printf("\n----FRAME----\n");
    printf("Frame ID: %s\n",PCNT.header.frameId);
    printf("Flags: %u %u\n",PCNT.header.flags[0],PCNT.header.flags[1]);
    printf("Counter: %s\n",PCNT.counter.string);
}
void FramesV2_freePCNT(ID3v2PCNTFrameType **PCNT){
    TxtStr_freeTextString(&(*PCNT)->counter);
    free(*PCNT);
    *PCNT = NULL;
}

void FramesV2_storeDefaultFrame(FILE* mp3FilePointer, uint32_t frameSize, ID3v2DefaultFrameType **DefaultFrame){
    *DefaultFrame = (ID3v2DefaultFrameType *)malloc(sizeof(ID3v2DefaultFrameType));
    (*DefaultFrame)->frameData = (uint8_t *)malloc(frameSize);
    fread((*DefaultFrame)->frameData, frameSize, 1, mp3FilePointer);
}
void FramesV2_printDefaultFrame(ID3v2DefaultFrameType DefaultFrame){
    printf("\n----FRAME----\n");
    printf("Frame ID: %s\n",DefaultFrame.header.frameId);
    printf("Flags: %u %u\n",DefaultFrame.header.flags[0],DefaultFrame.header.flags[1]);
}
void FramesV2_freeDefaultFrame(ID3v2DefaultFrameType **DefaultFrame){
    free((*DefaultFrame)->frameData);
    free(*DefaultFrame);
    *DefaultFrame = NULL;
}

void FramesV2_storeWWWF(FILE* mp3FilePointer, uint32_t frameSize,ID3v2WWWFrameType *WWWF){
    uint8_t *frameContent = (uint8_t *)malloc(frameSize);
    fread(frameContent, frameSize, 1, mp3FilePointer);

    char *urlPtr = (char *)frameContent;
    TxtStr_storeTextString(&(WWWF)->url,urlPtr,frameSize);
    free(frameContent);
}
void FramesV2_printWWWF(ID3v2WWWFrameType frame){
    printf("\n----FRAME----\n");
    printf("Frame ID: %s\n",frame.header.frameId);
    printf("Flags: %u %u\n",frame.header.flags[0],frame.header.flags[1]);
    printf("URL: ");
    for (int i = 0; i < (int)frame.url.size; i++) {
        if(frame.url.string[i] == '\0') printf(" ");
        else putchar(frame.url.string[i]);
    }
    printf("\n");
}
void FramesV2_freeWWWF(ID3v2WWWFrameType **WWWF){
    TxtStr_freeTextString(&(*WWWF)->url);
    free(*WWWF);
    *WWWF = NULL;
}

void FramesV2_storeWXXX(FILE *mp3FilePointer, uint32_t frameSize,ID3v2WXXXFrameType *WXXX){
    uint8_t *frameContent = (uint8_t *)malloc(frameSize);
    fread(frameContent, frameSize, 1, mp3FilePointer);

    size_t index = 0;
    WXXX->textEncoding = frameContent[index++];
    char *descPtr = (char *)frameContent + index;
    size_t descSize = strlen(descPtr)+1; 
    TxtStr_storeTextString(&WXXX->description,descPtr, descSize);
    index += descSize;
    char *urlPtr = (char *)frameContent + index;
    size_t urlSize = frameSize - index; 
    TxtStr_storeTextString(&WXXX->url,urlPtr, urlSize);
    free(frameContent);
}
void FramesV2_printWXXX(ID3v2WXXXFrameType frame){
    printf("\n----FRAME----\n");
    printf("Frame ID: %s\n",frame.header.frameId);
    printf("Flags: %u %u\n",frame.header.flags[0],frame.header.flags[1]);
    printf("TextEncoding: %d\n",frame.textEncoding);
    printf("Description: ");
    for (int i = 0; i < (int) frame.description.size; i++) {
        if(frame.description.string[i] == '\0') printf(" ");
        else putchar(frame.description.string[i]);
    }
    printf("\n");
    printf("url: ");
    for (int i = strlen(frame.url.string)+1; i < (int)frame.url.size; i++) {
        if(frame.url.string[i] == '\0') printf(" ");
        else putchar(frame.url.string[i]);
    }
    printf("\n");
}
void FramesV2_freeWXXX(ID3v2WXXXFrameType **WXXX){
        TxtStr_freeTextString(&(*WXXX)->description);
        TxtStr_freeTextString(&(*WXXX)->url);
        free(*WXXX);
        *WXXX = NULL;
}