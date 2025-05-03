#include "ID3v2Frames.h"
#include "SizeReader.h"
#include <string.h>
#include <ctype.h>

static void cleanInputBuffer(){
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
}

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
ID3v2APICFrameType *FramesV2_getAPIC(int version){
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

void FramesV2_storeTXTF(FILE *mp3FilePointer, uint32_t frameSize, ID3v2TXTFrameType *frame){
    uint8_t *frameContent = (uint8_t *)malloc(frameSize);
    fread(frameContent, frameSize, 1, mp3FilePointer);

    frame->textEncoding = frameContent[0];
    char *contentPtr = (char *)frameContent + 1;
    TxtStr_storeTextString(&frame->content,contentPtr, frameSize-1);
    free(frameContent);
}
ID3v2TXTFrameType * FramesV2_getTXXX(int version){
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
ID3v2TXTFrameType * FramesV2_getTXTF(char * frameID, int version){
    char content[255];

    ID3v2TXTFrameType *TXTFramePtr = (ID3v2TXTFrameType*) malloc(sizeof(ID3v2TXTFrameType));
    for (int i = 0; i < 4; i++) frameID[i] = toupper(frameID[i]);
    memcpy(TXTFramePtr->header.frameId,frameID,4); 
    TXTFramePtr->header.flags[0] = 0;TXTFramePtr->header.flags[1] = 0;
    // TXTFramePtr->textEncoding = 0;
    TXTFramePtr->textEncoding = 3;
    printf("Insert tag content (max. size 254): ");
    fgets(content, sizeof(content), stdin);
    content[strcspn(content, "\n")] = 0;
    printf("\n");
    TxtStr_storeTextString(&TXTFramePtr->content,content,strlen(content)+1);
    FramesV2_updateFrameSize(version,&TXTFramePtr->header,TXTFramePtr->content.size + 1);
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
        "TSSE", "TBPM", "TCOM", "TCON", "TCOP", "TDAT", "TDLY", "TENC", "TEXT", "TFLT", 
        "TIME", "TIT1", "TIT2", "TIT3", "TKEY", "TLAN", "TLEN", "TMED", "TOAL", "TOFN", 
        "TOLY", "TOPE", "TORY", "TOWN", "TPE1", "TPE2", "TPE3", "TPE4", "TPOS", "TPUB", 
        "TRCK", "TRDA", "TRSN", "TRSO", "TSIZ", "TSRC", "TYER", "TALB"
    };
    for (int i = 0; i < 38; i++) {
        if (strncasecmp(str, frames[i],4) == 0) return true;  
    }
    return false;
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
ID3v2COMMFrameType* FramesV2_getCOMM(int version){
    char description[65];
    char content[255];
    char language[4];
    ID3v2COMMFrameType *COMMFramePtr = (ID3v2COMMFrameType*) malloc(sizeof(ID3v2COMMFrameType));
    COMMFramePtr->header.flags[0] = 0;COMMFramePtr->header.flags[1] = 0;
    memcpy(COMMFramePtr->header.frameId,"COMM",4); 
    COMMFramePtr->textEncoding = 3;
    printf("Insert tag language (3 characters): ");
    fgets(language, sizeof(language), stdin);
    cleanInputBuffer();
    language[strcspn(language, "\n")] = 0;
    COMMFramePtr->language[0] = language[0];
    COMMFramePtr->language[1] = language[1];
    COMMFramePtr->language[2] = language[2];//Language should not have '\0'
    printf("\n");
    printf("Insert tag content description (64 characters): ");
    fgets(description, sizeof(description), stdin);
    description[strcspn(description, "\n")] = 0;
    TxtStr_storeTextString(&COMMFramePtr->contentDescript ,description,strlen(description)+1); 
    printf("\n");
    
    printf("Insert tag content  (254 characters): ");
    fgets(content, sizeof(content), stdin);
    content[strcspn(content, "\n")] = 0;
    TxtStr_storeTextString(&COMMFramePtr->actualText ,content,strlen(content)+1); 
    printf("\n");

    size_t size = 1 + 3 + COMMFramePtr->contentDescript.size + COMMFramePtr->actualText.size;
    FramesV2_updateFrameSize(version,&COMMFramePtr->header,size);
    return COMMFramePtr;
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

void FramesV2_storeMDCI(FILE *mp3FilePointer, uint32_t frameSize,ID3v2MCDIFrameType* MDCI){
    // *MDCI = (ID3v2MCDIFrameType *)malloc(sizeof(ID3v2MCDIFrameType));
    uint8_t *frameContent = (uint8_t *)malloc(frameSize);
    fread(frameContent, frameSize, 1, mp3FilePointer);
    char *ptr = (char *)frameContent;
    TxtStr_storeTextString(&(MDCI)->CDTOC,ptr, frameSize);
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

void FramesV2_storeDefaultFrame(FILE* mp3FilePointer, uint32_t frameSize, ID3v2DefaultFrameType *DefaultFrame){
    (DefaultFrame)->frameData = (uint8_t *)malloc(frameSize);
    fread((DefaultFrame)->frameData, frameSize, 1, mp3FilePointer);
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
ID3v2WWWFrameType * FramesV2_getWWWF(char * frameID, int version){
    char url[255];
    ID3v2WWWFrameType *WWWFramePtr = (ID3v2WWWFrameType*) malloc(sizeof(ID3v2WWWFrameType));
    for (int i = 0; i < 4; i++) frameID[i] = toupper(frameID[i]);
    memcpy(WWWFramePtr->header.frameId,frameID,4); 
    WWWFramePtr->header.flags[0] = 0;WWWFramePtr->header.flags[1] = 0;

    printf("Insert tag url (max. size 254): ");
    fgets(url, sizeof(url), stdin);
    url[strcspn(url, "\n")] = 0;
    printf("\n");
    TxtStr_storeTextString(&WWWFramePtr->url,url,strlen(url)+1);
    FramesV2_updateFrameSize(version,&WWWFramePtr->header,WWWFramePtr->url.size);
    return WWWFramePtr;
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
bool FramesV2_validWebFrameId(char *str) {
    const char *frames[] = {
        "WCOM", "WCOP", "WOAF", "WOAR", "WOAS", "WORS", "WPAY", "WPUB"
    };
    for (int i = 0; i < 8; i++) {
        if (strncasecmp(str, frames[i],4) == 0) return true;  
    }
    return false;
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
ID3v2WXXXFrameType * FramesV2_getWXXX(int version){
    ID3v2WXXXFrameType *WXXXFramePtr = (ID3v2WXXXFrameType*) malloc(sizeof(ID3v2WXXXFrameType));
    char url[255];
    char description[255];

    WXXXFramePtr->textEncoding = 3;
    memcpy(WXXXFramePtr->header.frameId,"WXXX",4); 
    WXXXFramePtr->header.flags[0] = 0;WXXXFramePtr->header.flags[1] = 0;

    printf("Insert tag description (max. size 254): ");
    fgets(description, sizeof(description), stdin);
    description[strcspn(description, "\n")] = 0;
    printf("\n");
    TxtStr_storeTextString(&WXXXFramePtr->description,description,strlen(description)+1);

    printf("Insert tag url (max. size 254): ");
    fgets(url, sizeof(url), stdin);
    url[strcspn(url, "\n")] = 0;
    TxtStr_storeTextString(&WXXXFramePtr->url,url,strlen(url)+1);
    printf("\n");
    FramesV2_updateFrameSize(version,&WXXXFramePtr->header,WXXXFramePtr->url.size+WXXXFramePtr->description.size+1);
    return WXXXFramePtr;
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
    for (int i = 0; i < (int)frame.url.size; i++) {
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