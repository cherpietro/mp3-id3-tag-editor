#include "GetFrameManager.h"
#include <ctype.h>
#include <string.h>

#define GET_TXTSTR(framePtr,buffer,txtStr)\
    fgets(buffer, sizeof(buffer), stdin);\
    cleanInputBuffer();\
    buffer[strcspn(buffer, "\n")] = 0;\
    TxtStr_storeTextString(&framePtr->txtStr ,buffer,strlen(buffer)+1); \

static void cleanInputBuffer(){
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
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
ID3v2WWWFrameType * GetFrame_WWWF(char * frameID, int version){
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
ID3v2WXXXFrameType * GetFrame_WXXX(int version){
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
ID3v2TXTFrameType * GetFrame_TXTF(char * frameID, int version){
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
ID3v2COMMFrameType* GetFrame_COMM(int version){
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
    GET_TXTSTR(COMMFramePtr,description,contentDescript);
    printf("\n");
    printf("Insert tag content  (254 characters): ");
    GET_TXTSTR(COMMFramePtr,content,actualText);
    printf("\n");
    size_t size = 1 + 3 + COMMFramePtr->contentDescript.size + COMMFramePtr->actualText.size;
    FramesV2_updateFrameSize(version,&COMMFramePtr->header,size);
    return COMMFramePtr;
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
