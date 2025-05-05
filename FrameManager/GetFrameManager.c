#include "GetFrameManager.h"
#include <ctype.h>
#include <string.h>

#define GET_TXTSTR(framePtr,buffer,txtStr)\
    fgets(buffer, sizeof(buffer), stdin);\
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
static void uint32_to_char_array(uint32_t num, char arr[4]) {
    arr[0] = (num >> 24) & 0xFF;
    arr[1] = (num >> 16) & 0xFF;
    arr[2] = (num >> 8)  & 0xFF;
    arr[3] = num & 0xFF;
}
ID3v2POPMFrameType* GetFrame_POPM(int version){
    char email[65];
    int rating;  
    long counter;  

    ID3v2POPMFrameType *POPMFramePtr = (ID3v2POPMFrameType*) malloc(sizeof(ID3v2POPMFrameType));
    memcpy(POPMFramePtr->header.frameId,"POPM",4); 
    POPMFramePtr->header.flags[0] = 0;POPMFramePtr->header.flags[1] = 0;
    printf("Insert user email (64 characters): ");
    GET_TXTSTR(POPMFramePtr,email,userEmail);
    printf("\n");
    do {
        printf("Insert rating (0-255): ");
        if (scanf("%d", &rating) != 1) {
            cleanInputBuffer();
            free(POPMFramePtr);
            return NULL;
        }
    }while(rating < 0 || rating > 255);  
    POPMFramePtr->rating = (uint8_t) rating;  
    do{
        printf("Insert counter (0 - 4294967295): ");
        if (scanf("%ld", &counter) != 1) {
            free(POPMFramePtr);
            cleanInputBuffer();
            return NULL;
        }
    }while(counter < 0 || counter > 4294967295);
    
    uint32_to_char_array(counter,POPMFramePtr->counter);  

    size_t size = POPMFramePtr->userEmail.size + 5;
    FramesV2_updateFrameSize(version,&POPMFramePtr->header,size);
    return POPMFramePtr;
}
ID3v2PCNTFrameType* GetFrame_PCNT(int version){
    long counter;  

    ID3v2PCNTFrameType *PCNTFramePtr = (ID3v2PCNTFrameType*) malloc(sizeof(ID3v2PCNTFrameType));
    memcpy(PCNTFramePtr->header.frameId,"PCNT",4); 
    PCNTFramePtr->header.flags[0] = 0;PCNTFramePtr->header.flags[1] = 0;  
    do{
        printf("Insert counter (0 - 4294967295): ");
        if (scanf("%ld", &counter) != 1) {
            free(PCNTFramePtr);
            cleanInputBuffer();
            return NULL;
        }
    }while(counter < 0 || counter > 4294967295);
    
    uint32_to_char_array(counter,PCNTFramePtr->counter);  

    size_t size = 4;
    FramesV2_updateFrameSize(version,&PCNTFramePtr->header,size);
    return PCNTFramePtr;
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
ID3v2TXXXFrameType * GetFrame_TXXX(int version){
    char description[65];
    char value[255];
    ID3v2TXXXFrameType *TXXXFramePtr = (ID3v2TXXXFrameType*) malloc(sizeof(ID3v2TXXXFrameType));
    memcpy(TXXXFramePtr->header.frameId,"TXXX",4); 
    TXXXFramePtr->header.flags[0] = 0;TXXXFramePtr->header.flags[1] = 0;
    TXXXFramePtr->textEncoding = 3;
    printf("Insert tag description (64 characters): ");
    GET_TXTSTR(TXXXFramePtr,description,description);
    printf("\n");    
    printf("Insert tag content (max. size 254): ");
    GET_TXTSTR(TXXXFramePtr,value,value);

    FramesV2_updateFrameSize(version,&TXXXFramePtr->header,TXXXFramePtr->description.size+TXXXFramePtr->value.size+1);
    return TXXXFramePtr;
}

ID3v2IPLSFrameType * GetFrame_IPLS(int version){
    char list[1501];
    
    ID3v2IPLSFrameType *IPLSFramePtr = (ID3v2IPLSFrameType*) malloc(sizeof(ID3v2IPLSFrameType));
    IPLSFramePtr->header.flags[0] = 0;IPLSFramePtr->header.flags[1] = 0;
    memcpy(IPLSFramePtr->header.frameId,"IPLS",4); 
    IPLSFramePtr->textEncoding = 3;
    printf("Insert people list in one line (1500 characters): ");
    GET_TXTSTR(IPLSFramePtr,list,peopeList);
    printf("\n");
    FramesV2_updateFrameSize(version,&IPLSFramePtr->header,IPLSFramePtr->peopeList.size+1);
    return IPLSFramePtr;
}
ID3v2USERFrameType * GetFrame_USER(int version){
    char language[4];
    char terms[501];
    ID3v2USERFrameType *USERFramePtr = (ID3v2USERFrameType*) malloc(sizeof(ID3v2USERFrameType));
    USERFramePtr->header.flags[0] = 0;USERFramePtr->header.flags[1] = 0;
    memcpy(USERFramePtr->header.frameId,"USER",4); 
    USERFramePtr->textEncoding = 3;
    printf("Insert tag language (3 characters): ");
    fgets(language, sizeof(language), stdin);
    cleanInputBuffer();
    language[strcspn(language, "\n")] = 0;
    USERFramePtr->language[0] = language[0];
    USERFramePtr->language[1] = language[1];
    USERFramePtr->language[2] = language[2];//Language should not have '\0'
    printf("Insert terms of use (500 characters): ");
    GET_TXTSTR(USERFramePtr,terms,actualText);
    printf("\n");
    FramesV2_updateFrameSize(version,&USERFramePtr->header,USERFramePtr->actualText.size+4);
    return USERFramePtr;
}
