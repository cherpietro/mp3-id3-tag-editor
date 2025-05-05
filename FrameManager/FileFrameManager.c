#include "FileFrameManager.h"
#include "../ID3v2Header.h"

#define WRITTE_FRAMELIST(Listframe,FramePtr,FrameType,writteFunction)\
    FrameType *FramePtr;\
    ListFramePtr_setFirstActive(Listframe);\
    while(Listframe->active != NULL){\
        FramePtr = (FrameType *)ListFramePtr_getActiveFramePtr(*Listframe);\
        writteFunction(destFilePtr, FramePtr);\
        ListFramePtr_setNextActive(Listframe);\
    }


static void writteTXXX(FILE *destFilePtr,ID3v2TXXXFrameType *TXXXFrame){
    fwrite(&TXXXFrame->header,1, sizeof(TXXXFrame->header),destFilePtr);
    fwrite(&TXXXFrame->textEncoding,1, 1,destFilePtr);
    fwrite(TXXXFrame->description.string,1, TxtStr_getStringLen(TXXXFrame->description),destFilePtr);
    fwrite(TXXXFrame->value.string,1, TxtStr_getStringLen(TXXXFrame->value),destFilePtr);
}
static void writteTXT(FILE *destFilePtr,ID3v2TXTFrameType *TXTFrame){
    fwrite(&TXTFrame->header,1, sizeof(TXTFrame->header),destFilePtr);
    fwrite(&TXTFrame->textEncoding,1, 1,destFilePtr);
    fwrite(TXTFrame->content.string,1, TxtStr_getStringLen(TXTFrame->content),destFilePtr);
}
static void writteAPIC(FILE *destFilePtr,ID3v2APICFrameType *APICFrame){
    fwrite(&APICFrame->header,1, sizeof(APICFrame->header),destFilePtr);
    fwrite(&APICFrame->textEncoding,1, 1,destFilePtr);
    fwrite(APICFrame->mimeType.string,1, TxtStr_getStringLen(APICFrame->mimeType),destFilePtr);
    fwrite(&APICFrame->pictureType,1, 1,destFilePtr);
    fwrite(APICFrame->description.string,1, TxtStr_getStringLen(APICFrame->description),destFilePtr);
    fwrite(APICFrame->imageData,1,APICFrame->imageDataSize ,destFilePtr);
}
static void writteWWW(FILE *destFilePtr,ID3v2WWWFrameType *WWWFrame){
    fwrite(&WWWFrame->header,1, sizeof(WWWFrame->header),destFilePtr);
    fwrite(&WWWFrame->url.string,1, TxtStr_getStringLen(WWWFrame->url),destFilePtr);
}
static void writteWXXX(FILE *destFilePtr,ID3v2WXXXFrameType *WXXXFrame){
    fwrite(&WXXXFrame->header,1, sizeof(WXXXFrame->header),destFilePtr);
    fwrite(&WXXXFrame->textEncoding,1, 1,destFilePtr);
    fwrite(WXXXFrame->description.string,1, TxtStr_getStringLen(WXXXFrame->description),destFilePtr);
    fwrite(&WXXXFrame->url.string,1, TxtStr_getStringLen(WXXXFrame->url),destFilePtr);
}
static void writteCOMM(FILE *destFilePtr,ID3v2COMMFrameType *COMMFrame){
    fwrite(&COMMFrame->header,1, sizeof(COMMFrame->header),destFilePtr);
    fwrite(&COMMFrame->textEncoding,1, 1,destFilePtr);
    fwrite(&COMMFrame->language,1, 3,destFilePtr);
    fwrite(COMMFrame->contentDescript.string,1, TxtStr_getStringLen(COMMFrame->contentDescript),destFilePtr);
    fwrite(COMMFrame->actualText.string,1, TxtStr_getStringLen(COMMFrame->actualText),destFilePtr);
}
static void writtePOPM(FILE *destFilePtr,ID3v2POPMFrameType *POPMFrame){
    fwrite(&POPMFrame->header,1, sizeof(POPMFrame->header),destFilePtr);
    fwrite(POPMFrame->userEmail.string,1, TxtStr_getStringLen(POPMFrame->userEmail),destFilePtr);
    fwrite(&POPMFrame->rating,1, 1,destFilePtr);
    fwrite(&POPMFrame->counter,1, 4,destFilePtr);
}
//PCNT
static void writtePRIV(FILE *destFilePtr,ID3v2PRIVFrameType *PRIVFrame){
    fwrite(&PRIVFrame->header,1, sizeof(PRIVFrame->header),destFilePtr);
    fwrite(PRIVFrame->owner.string,1, TxtStr_getStringLen(PRIVFrame->owner),destFilePtr);
    fwrite(PRIVFrame->privateData.string,1, TxtStr_getStringLen(PRIVFrame->privateData),destFilePtr);
}
static void writteDefault(FILE *destFilePtr,ID3v2DefaultFrameType *Defaultrame){
    fwrite(&Defaultrame->header,1, sizeof(Defaultrame->header),destFilePtr);
    fwrite(&Defaultrame->frameData,1, Defaultrame->size,destFilePtr);
}
////////////////////
void FileManager_writteTXXXFramesInFile(FILE *destFilePtr, ListFramePtr *TXXXFrameList){
    WRITTE_FRAMELIST(TXXXFrameList,TXXXFrame,ID3v2TXXXFrameType,writteTXXX);
}
void FileManager_writteTXTFramesInFile(FILE *destFilePtr, ListFramePtr *TXTFrameList){
    WRITTE_FRAMELIST(TXTFrameList,TXTFrame,ID3v2TXTFrameType,writteTXT);
}
void FileManager_writteAPICFramesInFile(FILE *destFilePtr, ListFramePtr *APICFrameList){
    WRITTE_FRAMELIST(APICFrameList,APICFrame,ID3v2APICFrameType,writteAPIC);
}
void FileManager_writteWWWFramesInFile(FILE *destFilePtr, ListFramePtr *WWWFrameList){
    WRITTE_FRAMELIST(WWWFrameList,WWWFrame,ID3v2WWWFrameType,writteWWW);
}

void FileManager_writteWXXXFramesInFile(FILE *destFilePtr, ListFramePtr *WXXXFrameList){
    WRITTE_FRAMELIST(WXXXFrameList,WXXXFrame,ID3v2WXXXFrameType,writteWXXX);
}
void FileManager_writteCOMMFramesInFile(FILE *destFilePtr, ListFramePtr *COMMFrameList){
    WRITTE_FRAMELIST(COMMFrameList,COMMFrame,ID3v2COMMFrameType,writteCOMM);
}
void FileManager_writtePOPMFramesInFile(FILE *destFilePtr, ListFramePtr *POPMFrameList){
    WRITTE_FRAMELIST(POPMFrameList,POPMFrame,ID3v2POPMFrameType,writtePOPM);
}
void FileManager_writtePCNTInFile(FILE *destFilePtr, ID3v2PCNTFrameType PCNT){ //TEST
    fwrite(&PCNT.header,1, sizeof(PCNT.header),destFilePtr);
    fwrite(PCNT.counter,4, sizeof(char),destFilePtr);
}

void FileManager_writtePRIVFramesInFile(FILE *destFilePtr, ListFramePtr *PRIVFrameList){
    WRITTE_FRAMELIST(PRIVFrameList,PRIVFrame,ID3v2PRIVFrameType,writtePRIV);
}
void FileManager_writteMCDIFrameInFile(FILE *destFilePtr, ID3v2MCDIFrameType MCDI){
    fwrite(&MCDI.header,1, sizeof(MCDI.header),destFilePtr);
    fwrite(MCDI.CDTOC.string,1, TxtStr_getStringLen(MCDI.CDTOC),destFilePtr);    
}
void FileManager_writteDefaultFramesInFile(FILE *destFilePtr, ListFramePtr *DefaultFrameList){
    WRITTE_FRAMELIST(DefaultFrameList,Defaultrame,ID3v2DefaultFrameType,writteDefault);
}
void FileManager_writteDefaultFrameInFile(FILE *destFilePtr, ID3v2DefaultFrameType Defaultrame){
    fwrite(&Defaultrame.header,1, sizeof(Defaultrame.header),destFilePtr);
    fwrite(&Defaultrame.frameData,1, Defaultrame.size,destFilePtr);
}
void FileManager_writteIPLSInFile(FILE *destFilePtr, ID3v2IPLSFrameType IPLS){
    fwrite(&IPLS.header,1, sizeof(IPLS.header),destFilePtr);
    fwrite(&IPLS.textEncoding,1, 1,destFilePtr);
    fwrite(IPLS.peopeList.string,1, TxtStr_getStringLen(IPLS.peopeList),destFilePtr);
}
void FileManager_writteUSERInFile(FILE *destFilePtr, ID3v2USERFrameType USER){
    fwrite(&USER.header,1, sizeof(USER.header),destFilePtr);
    fwrite(&USER.textEncoding,1, 1,destFilePtr);
    fwrite(&USER.language,1, 3,destFilePtr);
    fwrite(USER.actualText.string,1, TxtStr_getStringLen(USER.actualText),destFilePtr);
}
//USER
//RVRB
////////////////////////////

void FileManager_writtePadding(FILE *destFilePtr, int paddingSize){
    char zero = 0;
    for (int i = 0; i < paddingSize; i++) {
            fwrite(&zero, 1, 1, destFilePtr);
    }
}

bool FileManager_removeTagFromFile(char*file){
    FILE *mp3FilePointer = fopen(file,"r");
    if (mp3FilePointer) {
        fseek(mp3FilePointer, 0, SEEK_SET);
        uint32_t fileSize = ftell(mp3FilePointer);
        ID3v2HeaderType header;
        HeaderV2_storeHeader(mp3FilePointer,&header);
        if(HeaderV2_isID3v2Tag(header)){
                uint32_t tagSize = HeaderV2_getTagSize(header);
                fseek(mp3FilePointer, tagSize+10, SEEK_SET);
                uint32_t remainingSize = fileSize - tagSize;
                unsigned char *dataBuffer = (unsigned char *)malloc(remainingSize);
                if (!dataBuffer) {
                        printf("error\n");
                        return false;
                }
                size_t bytesRead = fread(dataBuffer, 1, remainingSize, mp3FilePointer);
                FILE *temp = fopen("savedFiles/tagRemoved.mp3", "wb");
                if (!temp) {
                    printf("error\n");
                    free(dataBuffer);
                    return false;
                }
                fwrite(dataBuffer, 1, bytesRead, temp);
                fclose(temp);
                free(dataBuffer);
                return true;
                // remove(file);
                // rename("tagRe6moved.mp3",file);
            }
        else printf("Not ID3v2 Tag detected or not yet supported version\n");
        fclose(mp3FilePointer);
        return false;

    }
    else printf("The file DOESN'T exist!\n");
    return false;
}

void FileManager_writteTagIntoFile(char *file, ID3TagType *ID3Tag){
    if(!FileManager_removeTagFromFile(file)) return;
    FILE *mp3FilePointer = fopen("./savedFiles/tagRemoved.mp3","r");
    if(mp3FilePointer){
        fseek(mp3FilePointer,0,SEEK_END);
        uint32_t fileSize = ftell(mp3FilePointer);
        fseek(mp3FilePointer,0,SEEK_SET);
        unsigned char *dataBuffer = (unsigned char *)malloc(fileSize);
        if(!dataBuffer){
            printf("error");
            fclose(mp3FilePointer);
            return;
        }
        fread(dataBuffer,1,fileSize,mp3FilePointer);
        FILE *temp = fopen("./savedFiles/modified.mp3","w");
        if(!temp){
            printf("error");
            fclose(temp);
            return;
        }
        // header
        fwrite(&ID3Tag->header,1,sizeof(ID3Tag->header),temp);

        FileManager_writteTXXXFramesInFile(temp,&ID3Tag->TXXXFrameList);
        FileManager_writteTXTFramesInFile(temp,&ID3Tag->TXTFrameList);
        FileManager_writteAPICFramesInFile(temp,&ID3Tag->APICFrameList);
        FileManager_writteWWWFramesInFile(temp,&ID3Tag->WWWFrameList);
        FileManager_writteWXXXFramesInFile(temp,&ID3Tag->WXXXFrameList);
        FileManager_writteCOMMFramesInFile(temp,&ID3Tag->COMMFrameList);
        FileManager_writtePOPMFramesInFile(temp,&ID3Tag->POPMFrameList);
        if(ID3Tag->PCNT != NULL) FileManager_writtePCNTInFile(temp,*ID3Tag->PCNT);
        FileManager_writtePRIVFramesInFile(temp,&ID3Tag->PRIVFrameList);
        if(ID3Tag->MCDI != NULL) FileManager_writteDefaultFrameInFile(temp,*ID3Tag->MCDI);
        FileManager_writteDefaultFramesInFile(temp,&ID3Tag->UFIDFrameList);
        FileManager_writteDefaultFramesInFile(temp,&ID3Tag->USLTFrameList);
        FileManager_writteDefaultFramesInFile(temp,&ID3Tag->SYLTFrameList);
        FileManager_writteDefaultFramesInFile(temp,&ID3Tag->GEOBFrameList);
        FileManager_writteDefaultFramesInFile(temp,&ID3Tag->LINKFrameList);
        FileManager_writteDefaultFramesInFile(temp,&ID3Tag->AENCFrameList);
        FileManager_writteDefaultFramesInFile(temp,&ID3Tag->ENCRFrameList);
        FileManager_writteDefaultFramesInFile(temp,&ID3Tag->GRIDFrameList);
        if(ID3Tag->IPLS != NULL) FileManager_writteIPLSInFile(temp,*ID3Tag->IPLS);
        if(ID3Tag->SYTC != NULL) FileManager_writteDefaultFrameInFile(temp,*ID3Tag->SYTC);
        if(ID3Tag->USER != NULL) FileManager_writteUSERInFile(temp,*ID3Tag->USER);
        //
        if(ID3Tag->OWNE != NULL) FileManager_writteDefaultFrameInFile(temp,*ID3Tag->OWNE);
        if(ID3Tag->COMR != NULL) FileManager_writteDefaultFrameInFile(temp,*ID3Tag->COMR);
        if(ID3Tag->POSS != NULL) FileManager_writteDefaultFrameInFile(temp,*ID3Tag->POSS);
        //RVRB
        if(ID3Tag->EQUA != NULL) FileManager_writteDefaultFrameInFile(temp,*ID3Tag->EQUA);
        if(ID3Tag->MLLT != NULL) FileManager_writteDefaultFrameInFile(temp,*ID3Tag->MLLT);
        if(ID3Tag->ETCO != NULL) FileManager_writteDefaultFrameInFile(temp,*ID3Tag->ETCO);
        if(ID3Tag->RVAD != NULL) FileManager_writteDefaultFrameInFile(temp,*ID3Tag->RVAD);

        FileManager_writtePadding(temp,ID3Tag->paddingSize);
        
        // content
        fwrite(dataBuffer,1,fileSize,temp);
        fclose(temp);
        // remove(file);
        // rename("temp.mp3",file);
    }
    remove("./savedFiles/tagRemoved.mp3");
}
