#include "ID3v2.h"
#include "SizeReader.h"
#include "FileFrameManager.h"
#include "PrintFrameManager.h"
#include "DeleteFrameManager.h"
#include "AddFrameManager.h"
#include "StoreFrameManager.h"
#include "SizeFrameManager.h"
#include <string.h>

void ID3v2_init(ID3TagType *ID3Tag){
    ListFramePtr_init(&ID3Tag->TXTFrameList);
    ListFramePtr_init(&ID3Tag->COMMFrameList);
    ListFramePtr_init(&ID3Tag->PRIVFrameList);
    ListFramePtr_init(&ID3Tag->APICFrameList);
    ListFramePtr_init(&ID3Tag->POPMFrameList);
    ListFramePtr_init(&ID3Tag->WWWFrameList);
    ListFramePtr_init(&ID3Tag->WXXXFrameList);
    ListFramePtr_init(&ID3Tag->UFIDFrameList);
    ListFramePtr_init(&ID3Tag->USLTFrameList);
    ListFramePtr_init(&ID3Tag->SYLTFrameList);
    ListFramePtr_init(&ID3Tag->GEOBFrameList);
    ListFramePtr_init(&ID3Tag->LINKFrameList);
    ListFramePtr_init(&ID3Tag->AENCFrameList);
    ListFramePtr_init(&ID3Tag->ENCRFrameList);
    ListFramePtr_init(&ID3Tag->GRIDFrameList);

    ID3Tag->MCDI = NULL;
    ID3Tag->RVRB = NULL;
    ID3Tag->POSS = NULL;
    ID3Tag->COMR = NULL;
    ID3Tag->EQUA = NULL;
    ID3Tag->MLLT = NULL;
    ID3Tag->ETCO = NULL;
    ID3Tag->RVAD = NULL;

    ID3Tag->IPLS = NULL;
    ID3Tag->SYTC = NULL;
    ID3Tag->USER = NULL;
    ID3Tag->OWNE = NULL;
    ID3Tag->PCNT = NULL;
}

void ID3v2_free(ID3TagType *ID3Tag){
    ListFramePtr_setFirstActive(&ID3Tag->TXTFrameList);
    while (!ListFramePtr_isEmpty(ID3Tag->TXTFrameList)) {
        ID3v2TXTFrameType* ptrTXT = (ID3v2TXTFrameType*) ListFramePtr_getActiveFramePtr(ID3Tag->TXTFrameList);
        FramesV2_freeTXTF(&ptrTXT);
        ListFramePtr_deleteActive(&ID3Tag->TXTFrameList);
    }
    ListFramePtr_setFirstActive(&ID3Tag->COMMFrameList);
    while (!ListFramePtr_isEmpty(ID3Tag->COMMFrameList)) {
        ID3v2COMMFrameType* ptrCOMM = (ID3v2COMMFrameType*) ListFramePtr_getActiveFramePtr(ID3Tag->COMMFrameList);
        FramesV2_freeCOMM(&ptrCOMM);
        ListFramePtr_deleteActive(&ID3Tag->COMMFrameList);
    }
    ListFramePtr_setFirstActive(&ID3Tag->PRIVFrameList);
    while (!ListFramePtr_isEmpty(ID3Tag->COMMFrameList)) {
        ID3v2PRIVFrameType* ptrPRIV = (ID3v2PRIVFrameType*) ListFramePtr_getActiveFramePtr(ID3Tag->PRIVFrameList);
        FramesV2_freePRIV(&ptrPRIV);
        ListFramePtr_deleteActive(&ID3Tag->PRIVFrameList);
    }
    ListFramePtr_setFirstActive(&ID3Tag->APICFrameList);
    while (!ListFramePtr_isEmpty(ID3Tag->APICFrameList)) {
        ID3v2APICFrameType* ptrAPIC = (ID3v2APICFrameType*) ListFramePtr_getActiveFramePtr(ID3Tag->APICFrameList);
        FramesV2_freeAPIC(&ptrAPIC);
        ListFramePtr_deleteActive(&ID3Tag->APICFrameList);
    }
    ListFramePtr_setFirstActive(&ID3Tag->POPMFrameList);
    while (!ListFramePtr_isEmpty(ID3Tag->POPMFrameList)) {
        ID3v2POPMFrameType* ptrPOPM = (ID3v2POPMFrameType*) ListFramePtr_getActiveFramePtr(ID3Tag->POPMFrameList);
        FramesV2_freePOPM(&ptrPOPM);
        ListFramePtr_deleteActive(&ID3Tag->POPMFrameList);
    }
    ListFramePtr_setFirstActive(&ID3Tag->WXXXFrameList);
    while (!ListFramePtr_isEmpty(ID3Tag->WXXXFrameList)) {
        ID3v2WXXXFrameType* ptrWXXX = (ID3v2WXXXFrameType*) ListFramePtr_getActiveFramePtr(ID3Tag->WXXXFrameList);
        FramesV2_freeWXXX(&ptrWXXX);
        ListFramePtr_deleteActive(&ID3Tag->WXXXFrameList);
    }
    ListFramePtr_setFirstActive(&ID3Tag->WWWFrameList);
    while (!ListFramePtr_isEmpty(ID3Tag->WWWFrameList)) {
        ID3v2WWWFrameType* ptrWWW = (ID3v2WWWFrameType*) ListFramePtr_getActiveFramePtr(ID3Tag->WWWFrameList);
        FramesV2_freeWWWF(&ptrWWW);
        ListFramePtr_deleteActive(&ID3Tag->WWWFrameList);
    }
    if(ID3Tag->MCDI != NULL) FramesV2_freeMCDI(&ID3Tag->MCDI);
    ListFramePtr_setFirstActive(&ID3Tag->UFIDFrameList);
    while (!ListFramePtr_isEmpty(ID3Tag->UFIDFrameList)) {
        ID3v2DefaultFrameType* defaultPtr = (ID3v2DefaultFrameType*) ListFramePtr_getActiveFramePtr(ID3Tag->UFIDFrameList);
        FramesV2_freeDefaultFrame(&defaultPtr);
        ListFramePtr_deleteActive(&ID3Tag->UFIDFrameList);
    }
    ListFramePtr_setFirstActive(&ID3Tag->USLTFrameList);
    while (!ListFramePtr_isEmpty(ID3Tag->USLTFrameList)) {
        ID3v2DefaultFrameType* defaultPtr = (ID3v2DefaultFrameType*) ListFramePtr_getActiveFramePtr(ID3Tag->USLTFrameList);
        FramesV2_freeDefaultFrame(&defaultPtr);
        ListFramePtr_deleteActive(&ID3Tag->USLTFrameList);
    }
    ListFramePtr_setFirstActive(&ID3Tag->SYLTFrameList);
    while (!ListFramePtr_isEmpty(ID3Tag->SYLTFrameList)) {
        ID3v2DefaultFrameType* defaultPtr = (ID3v2DefaultFrameType*) ListFramePtr_getActiveFramePtr(ID3Tag->SYLTFrameList);
        FramesV2_freeDefaultFrame(&defaultPtr);
        ListFramePtr_deleteActive(&ID3Tag->SYLTFrameList);
    }
    ListFramePtr_setFirstActive(&ID3Tag->GEOBFrameList);
    while (!ListFramePtr_isEmpty(ID3Tag->GEOBFrameList)) {
        ID3v2DefaultFrameType* defaultPtr = (ID3v2DefaultFrameType*) ListFramePtr_getActiveFramePtr(ID3Tag->GEOBFrameList);
        FramesV2_freeDefaultFrame(&defaultPtr);
        ListFramePtr_deleteActive(&ID3Tag->GEOBFrameList);
    }
    ListFramePtr_setFirstActive(&ID3Tag->LINKFrameList);
    while (!ListFramePtr_isEmpty(ID3Tag->LINKFrameList)) {
        ID3v2DefaultFrameType* defaultPtr = (ID3v2DefaultFrameType*) ListFramePtr_getActiveFramePtr(ID3Tag->LINKFrameList);
        FramesV2_freeDefaultFrame(&defaultPtr);
        ListFramePtr_deleteActive(&ID3Tag->LINKFrameList);
    }
    ListFramePtr_setFirstActive(&ID3Tag->AENCFrameList);
    while (!ListFramePtr_isEmpty(ID3Tag->AENCFrameList)) {
        ID3v2DefaultFrameType* defaultPtr = (ID3v2DefaultFrameType*) ListFramePtr_getActiveFramePtr(ID3Tag->AENCFrameList);
        FramesV2_freeDefaultFrame(&defaultPtr);
        ListFramePtr_deleteActive(&ID3Tag->AENCFrameList);
    }
    ListFramePtr_setFirstActive(&ID3Tag->ENCRFrameList);
    while (!ListFramePtr_isEmpty(ID3Tag->ENCRFrameList)) {
        ID3v2DefaultFrameType* defaultPtr = (ID3v2DefaultFrameType*) ListFramePtr_getActiveFramePtr(ID3Tag->ENCRFrameList);
        FramesV2_freeDefaultFrame(&defaultPtr);
        ListFramePtr_deleteActive(&ID3Tag->ENCRFrameList);
    }
    ListFramePtr_setFirstActive(&ID3Tag->GRIDFrameList);
    while (!ListFramePtr_isEmpty(ID3Tag->GRIDFrameList)) {
        ID3v2DefaultFrameType* defaultPtr = (ID3v2DefaultFrameType*) ListFramePtr_getActiveFramePtr(ID3Tag->GRIDFrameList);
        FramesV2_freeDefaultFrame(&defaultPtr);
        ListFramePtr_deleteActive(&ID3Tag->GRIDFrameList);
    }
    if(ID3Tag->COMR != NULL) FramesV2_freeDefaultFrame(&ID3Tag->COMR);
    if(ID3Tag->POSS != NULL) FramesV2_freeDefaultFrame(&ID3Tag->POSS);
    if(ID3Tag->RVRB != NULL) FramesV2_freeDefaultFrame(&ID3Tag->RVRB);
    if(ID3Tag->EQUA != NULL) FramesV2_freeDefaultFrame(&ID3Tag->EQUA);
    if(ID3Tag->MLLT != NULL) FramesV2_freeDefaultFrame(&ID3Tag->MLLT);
    if(ID3Tag->ETCO != NULL) FramesV2_freeDefaultFrame(&ID3Tag->ETCO);
    if(ID3Tag->RVAD != NULL) FramesV2_freeDefaultFrame(&ID3Tag->RVAD);
    if(ID3Tag->IPLS != NULL) FramesV2_freeDefaultFrame(&ID3Tag->IPLS);
    if(ID3Tag->SYTC != NULL) FramesV2_freeDefaultFrame(&ID3Tag->SYTC);
    if(ID3Tag->USER != NULL) FramesV2_freeDefaultFrame(&ID3Tag->USER);
    if(ID3Tag->OWNE != NULL) FramesV2_freeDefaultFrame(&ID3Tag->OWNE);
    if(ID3Tag->PCNT != NULL) FramesV2_freeDefaultFrame(&ID3Tag->PCNT);
}

void ID3v2_storeTagInStruct(char *file,ID3TagType *ID3Tag){
    FILE *mp3FilePointer = fopen(file,"r");
    if (mp3FilePointer) {
        int paddingReached = 0;
        HeaderV2_storeHeader(mp3FilePointer,&ID3Tag->header);
        if(HeaderV2_isID3v2Tag(ID3Tag->header)){
                HeaderV2_printTagHeader(ID3Tag->header);
                uint32_t tagSize = HeaderV2_getTagSize(ID3Tag->header);
                while(!paddingReached && ftell(mp3FilePointer) < tagSize + 10){
                    paddingReached = StoreFrame_storeNextFrameInStruct(mp3FilePointer,ID3Tag);
                }
                if (paddingReached) ID3Tag->paddingSize = (HeaderV2_getTagSize(ID3Tag->header))+10 - (ftell(mp3FilePointer))+10; //ID3Tag size doesn't include header 
        }
        else printf("Not ID3v2 Tag detected or not yet supported version\n");
        fclose(mp3FilePointer);
    }
    else printf("The file DOESN'T exist!\n");
}

//IMPLEMENT REMAINING TAGS
void ID3v2_getTagSizeOfTheStruct(ID3TagType *ID3Tag){
    size_t tagSizeOfStruct;
    printf("\nsize in ID3Tag: %d bytes\n",HeaderV2_getTagSize(ID3Tag->header));
    tagSizeOfStruct = SizeFrame_getFrameListSize(ID3Tag);    
    if((HeaderV2_getTagSize(ID3Tag->header) +10 - ID3Tag->paddingSize ) == tagSizeOfStruct ){
        printf("size is okay\n");
    }
    else{
        printf("size is NOT okay %ld\n",(tagSizeOfStruct+10 - (int) ID3Tag->paddingSize ));
    }
}

void ID3v2_deleteFrame(ID3TagType *ID3Tag, char *frameID){
    int deletedSize;
    int oldSize;
    if(strncasecmp(frameID,"TXXX",4)==0) deletedSize = DeleteFrame_deleteTXXX(&ID3Tag->TXTFrameList,ID3Tag->header.version[0]);
    else if(strncasecmp(frameID,"T",1)==0) deletedSize = DeleteFrame_deleteTXTF(&ID3Tag->TXTFrameList,frameID,ID3Tag->header.version[0]);
    else if(strncasecmp(frameID,"WXXX",4)==0) deletedSize = DeleteFrame_deleteWXXX(&ID3Tag->WXXXFrameList,ID3Tag->header.version[0]);
    else if(strncasecmp(frameID,"W",1)==0) deletedSize = DeleteFrame_deleteWWWF(&ID3Tag->WWWFrameList,frameID,ID3Tag->header.version[0]);
    else if(strncasecmp(frameID,"PRIV",4)==0) deletedSize = DeleteFrame_deletePRIV(&ID3Tag->PRIVFrameList,ID3Tag->header.version[0]);
    else if(strncasecmp(frameID,"COMM",4)==0) deletedSize = DeleteFrame_deleteCOMM(&ID3Tag->COMMFrameList,ID3Tag->header.version[0]);
    else if(strncasecmp(frameID,"APIC",4)==0) deletedSize = DeleteFrame_deleteAPIC(&ID3Tag->APICFrameList,ID3Tag->header.version[0]);
    else if(strncasecmp(frameID,"POPM",4)==0) deletedSize = DeleteFrame_deletePOPM(&ID3Tag->POPMFrameList,ID3Tag->header.version[0]);
    else if(strncasecmp(frameID,"MDCI",4)==0) deletedSize = DeleteFrame_deleteMDCI(&ID3Tag->MCDI,ID3Tag->header.version[0]);
    //NOT TESTED
    else if(strncasecmp(frameID,"RVRB",4)==0){deletedSize = DeleteFrame_default(&ID3Tag->RVRB,ID3Tag->header.version[0]);}
    else if(strncasecmp(frameID,"EQUA",4)==0){deletedSize = DeleteFrame_default(&ID3Tag->EQUA,ID3Tag->header.version[0]);}
    else if(strncasecmp(frameID,"MLLT",4)==0){deletedSize = DeleteFrame_default(&ID3Tag->MLLT,ID3Tag->header.version[0]);}
    else if(strncasecmp(frameID,"ETCO",4)==0){deletedSize = DeleteFrame_default(&ID3Tag->ETCO,ID3Tag->header.version[0]);}
    else if(strncasecmp(frameID,"RVAD",4)==0){deletedSize = DeleteFrame_default(&ID3Tag->RVAD,ID3Tag->header.version[0]);}
    else if(strncasecmp(frameID,"POSS",4)==0){deletedSize = DeleteFrame_default(&ID3Tag->POSS,ID3Tag->header.version[0]);}
    else if(strncasecmp(frameID,"COMR",4)==0){deletedSize = DeleteFrame_default(&ID3Tag->COMR,ID3Tag->header.version[0]);}
    else if(strncasecmp(frameID,"UFID",4)==0){deletedSize = DeleteFrame_defaultList(&ID3Tag->UFIDFrameList,ID3Tag->header.version[0]);}
    else if(strncasecmp(frameID,"USLT",4)==0){deletedSize = DeleteFrame_defaultList(&ID3Tag->USLTFrameList,ID3Tag->header.version[0]);}
    else if(strncasecmp(frameID,"SYLT",4)==0){deletedSize = DeleteFrame_defaultList(&ID3Tag->SYLTFrameList,ID3Tag->header.version[0]);}
    else if(strncasecmp(frameID,"GEOB",4)==0){deletedSize = DeleteFrame_defaultList(&ID3Tag->GEOBFrameList,ID3Tag->header.version[0]);}
    else if(strncasecmp(frameID,"LINK",4)==0){deletedSize = DeleteFrame_defaultList(&ID3Tag->LINKFrameList,ID3Tag->header.version[0]);}
    else if(strncasecmp(frameID,"AENC",4)==0){deletedSize = DeleteFrame_defaultList(&ID3Tag->AENCFrameList,ID3Tag->header.version[0]);}
    else if(strncasecmp(frameID,"ENCR",4)==0){deletedSize = DeleteFrame_defaultList(&ID3Tag->ENCRFrameList,ID3Tag->header.version[0]);}
    else if(strncasecmp(frameID,"GRID",4)==0){deletedSize = DeleteFrame_defaultList(&ID3Tag->GRIDFrameList,ID3Tag->header.version[0]);}
    else if(strncasecmp(frameID,"IPLS",4)==0){deletedSize = DeleteFrame_default(&ID3Tag->IPLS,ID3Tag->header.version[0]);}
    else if(strncasecmp(frameID,"SYTC",4)==0){deletedSize = DeleteFrame_default(&ID3Tag->SYTC,ID3Tag->header.version[0]);}
    else if(strncasecmp(frameID,"USER",4)==0){deletedSize = DeleteFrame_default(&ID3Tag->USER,ID3Tag->header.version[0]);}
    else if(strncasecmp(frameID,"OWNE",4)==0){deletedSize = DeleteFrame_default(&ID3Tag->OWNE,ID3Tag->header.version[0]);}
    else if(strncasecmp(frameID,"PCNT",4)==0){deletedSize = DeleteFrame_default(&ID3Tag->PCNT,ID3Tag->header.version[0]);}
    else return; //NOT SUPPORTED TAG
    oldSize = HeaderV2_getTagSize(ID3Tag->header);
    HeaderV2_updateTagSize(&ID3Tag->header,oldSize-deletedSize);

}

void ID3v2_addFrame(ID3TagType *ID3Tag, char *frameID){
    int incrementedSize = 0;
    int oldSize = 0;
    if(strncasecmp(frameID,"TXXX",4)==0){ 
        incrementedSize = AddFrame_addTXXX(&ID3Tag->TXTFrameList,ID3Tag->header.version[0]);
        oldSize = HeaderV2_getTagSize(ID3Tag->header);
        HeaderV2_updateTagSize(&ID3Tag->header,oldSize+incrementedSize);
    }    
    else if(strncasecmp(frameID,"T",1)==0){ 
        if(FramesV2_validTextFrameId(frameID)){
            incrementedSize = AddFrame_addTXTF(&ID3Tag->TXTFrameList,frameID,ID3Tag->header.version[0]);
            oldSize = HeaderV2_getTagSize(ID3Tag->header);
            HeaderV2_updateTagSize(&ID3Tag->header,oldSize+incrementedSize);
        }
        else printf("Not supported tag to include");
    }
    else if(strncasecmp(frameID,"WXXX",4)==0){
        incrementedSize = AddFrame_addWXXX(&ID3Tag->WXXXFrameList,ID3Tag->header.version[0]);
        oldSize = HeaderV2_getTagSize(ID3Tag->header);
        HeaderV2_updateTagSize(&ID3Tag->header,oldSize+incrementedSize);
    }
    else if(strncasecmp(frameID,"W",1)==0){
        if(FramesV2_validWebFrameId(frameID)){
            incrementedSize = AddFrame_addWWWF(&ID3Tag->WWWFrameList,frameID,ID3Tag->header.version[0]);
            oldSize = HeaderV2_getTagSize(ID3Tag->header);
            HeaderV2_updateTagSize(&ID3Tag->header,oldSize+incrementedSize);
        }
        else printf("Not supported tag to include");
    }
    else if(strncasecmp(frameID,"APIC",4)==0){
        incrementedSize = AddFrame_addAPIC(&ID3Tag->APICFrameList,ID3Tag->header.version[0]);
        oldSize = HeaderV2_getTagSize(ID3Tag->header);
        HeaderV2_updateTagSize(&ID3Tag->header,oldSize+incrementedSize);
    }
    else if(strncasecmp(frameID,"COMM",4)==0){
        incrementedSize = AddFrame_addCOMM(&ID3Tag->COMMFrameList,ID3Tag->header.version[0]);
        oldSize = HeaderV2_getTagSize(ID3Tag->header);
        HeaderV2_updateTagSize(&ID3Tag->header,oldSize+incrementedSize);
    }
}
