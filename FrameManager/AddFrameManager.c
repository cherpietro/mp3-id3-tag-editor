#include "AddFrameManager.h"
#include "PrintFrameManager.h"
#include "GetFrameManager.h"
#include "../ID3v2Frames.h"
#include <string.h>

#define OLD_ADD_FRAME_LIST(FrameList,FrameType,FramePtr,FrameGetFunct,FramePrintFunct)\
    FrameType *FramePtr = FrameGetFunct(version);\
    if(FramePtr == NULL)return 0;\
    ListFramePtr_insertLast(FrameList,FramePtr);\
    FramePrintFunct(*FramePtr,version);\
    return (FramesV2_getFrameSize(version,FramePtr->header) +10);

#define ADD_FRAME_LIST(FrameList,FrameType,FramePtr,FrameGetFunct,FramePrintFunct,FrameCheckFunct)\
    FrameType *FramePtr = FrameGetFunct(version);\
    if(FramePtr == NULL)return 0;\
    if(FrameCheckFunct(FrameList,FramePtr)){printf("repeated frame\n");free(FramePtr);return 0;}\
    ListFramePtr_insertLast(FrameList,FramePtr);\
    FramePrintFunct(*FramePtr,version);\
    return (FramesV2_getFrameSize(version,FramePtr->header) +10);

#define ADD_FRAME(FramePtr,FrameGetFunct,FramePrintFunct)\
    *FramePtr = FrameGetFunct(version);\
    if(*FramePtr == NULL)return 0;\
    FramePrintFunct(**FramePtr,version);\
    return (FramesV2_getFrameSize(version,(*FramePtr)->header) +10);

static void cleanInputBuffer(){
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
}

int AddFrame_addTXXX(ListFramePtr *TXTFrameList,int version){
    OLD_ADD_FRAME_LIST(TXTFrameList,ID3v2TXTFrameType,TXTFramePtr,GetFrame_TXXX,PrintFrame_TXTF);
}
static bool hasRepeatWXXX(ListFramePtr *WXXXrameList,ID3v2WXXXFrameType *WXXXFramePtr){
    ID3v2WXXXFrameType *checkPtr;
    ListFramePtr_setFirstActive(WXXXrameList);
    while(WXXXrameList->active != NULL){
        checkPtr= ListFramePtr_getActiveFramePtr(*WXXXrameList);
        if(strcmp(checkPtr->description.string,WXXXFramePtr->description.string) == 0) return true;
        ListFramePtr_setNextActive(WXXXrameList);
    }
    return false;
}
int AddFrame_addWXXX(ListFramePtr *WXXXFrameList,int version){
    ADD_FRAME_LIST(WXXXFrameList,ID3v2WXXXFrameType,WXXXFramePtr,GetFrame_WXXX,PrintFrame_WXXX,hasRepeatWXXX);

}
static bool hasRepeatAPIC(ListFramePtr *APICFrameList,ID3v2APICFrameType *APICFramePtr){
    ID3v2APICFrameType *checkPtr;
    ListFramePtr_setFirstActive(APICFrameList);
    while(APICFrameList->active != NULL){
        checkPtr= ListFramePtr_getActiveFramePtr(*APICFrameList);
        if(strcmp(checkPtr->description.string,APICFramePtr->description.string) == 0) return true;
        ListFramePtr_setNextActive(APICFrameList);
    }
    return false;
}
int AddFrame_addAPIC(ListFramePtr *APICFrameList,int version){
    ADD_FRAME_LIST(APICFrameList,ID3v2APICFrameType,APICFramePtr,GetFrame_APIC,PrintFrame_APIC,hasRepeatAPIC);
}

static bool hasRepeatCOMM(ListFramePtr *COMMFrameList,ID3v2COMMFrameType *COMMFramePtr){
    ID3v2COMMFrameType *checkPtr;
    ListFramePtr_setFirstActive(COMMFrameList);
    while(COMMFrameList->active != NULL){
        checkPtr= ListFramePtr_getActiveFramePtr(*COMMFrameList);
        if(strcmp(checkPtr->contentDescript.string,COMMFramePtr->contentDescript.string) == 0) return true;
        ListFramePtr_setNextActive(COMMFrameList);
    }
    return false;
}
int AddFrame_addCOMM(ListFramePtr *COMMFrameList,int version){
    ADD_FRAME_LIST(COMMFrameList,ID3v2COMMFrameType,COMMFramePtr,GetFrame_COMM,PrintFrame_COMM,hasRepeatCOMM);
}
static bool hasRepeatPOPM(ListFramePtr *POPMFrameList,ID3v2POPMFrameType *POPMFramePtr){
    ID3v2POPMFrameType *checkPtr;
    ListFramePtr_setFirstActive(POPMFrameList);
    while(POPMFrameList->active != NULL){
        checkPtr= ListFramePtr_getActiveFramePtr(*POPMFrameList);
        if(strcmp(checkPtr->userEmail.string,POPMFramePtr->userEmail.string) == 0) return true;
        ListFramePtr_setNextActive(POPMFrameList);
    }
    return false;
}
int AddFrame_addPOPM(ListFramePtr *POPMFrameList,int version){
    ADD_FRAME_LIST(POPMFrameList,ID3v2POPMFrameType,POPMFramePtr,GetFrame_POPM,PrintFrame_POPM,hasRepeatPOPM);
}
int AddFrame_addPCNT(ID3v2PCNTFrameType **PCNT,int version){
    ADD_FRAME(PCNT,GetFrame_PCNT,PrintFrame_PCNT);
}
int AddFrame_addIPLS(ID3v2IPLSFrameType **IPLS,int version){
    ADD_FRAME(IPLS,GetFrame_IPLS,PrintFrame_IPLS);
}

int AddFrame_addTXTF(ListFramePtr *TXTFrameList,char *frameID,int version){
    char option;
    int deletedSize = 0;
    int incrementedSize = 0;
    ListFramePtr_setFirstActive(TXTFrameList);
    ID3v2TXTFrameType * TXTFramePtr = ListFramePtr_getActiveFramePtr(*TXTFrameList);
    while (TXTFramePtr != NULL && strncasecmp(frameID,TXTFramePtr->header.frameId,4) != 0){
        ListFramePtr_setNextActive(TXTFrameList);
        TXTFramePtr = ListFramePtr_getActiveFramePtr(*TXTFrameList);
    }
    if(TXTFramePtr != NULL) {
        PrintFrame_TXTF(*TXTFramePtr,version);
        printf("\n\nFRAME ALREADY EXIST. Want to delete this frame? (y/n): ");
        do{
            option = getchar();
            cleanInputBuffer();
        }while(option != 'y' && option != 'Y' && option != 'n' && option != 'N');
        if(option == 'n' || option == 'N') { return 0;}
        deletedSize = FramesV2_getFrameSize(version,TXTFramePtr->header)+10;
        FramesV2_freeTXTF(&TXTFramePtr);
        ListFramePtr_deleteActive(TXTFrameList);
        incrementedSize -= deletedSize;
    }
    TXTFramePtr = GetFrame_TXTF(frameID,version);
    ListFramePtr_insertLast(TXTFrameList,TXTFramePtr);
    incrementedSize += FramesV2_getFrameSize(version,TXTFramePtr->header);
    PrintFrame_TXTF(*TXTFramePtr,version);
    return (incrementedSize + 10);
}

int AddFrame_addWWWF(ListFramePtr *WWWFrameList,char *frameID,int version){
    char option;
    int deletedSize = 0;
    int incrementedSize = 0;
    ListFramePtr_setFirstActive(WWWFrameList);
    ID3v2WWWFrameType * WWWFramePtr = ListFramePtr_getActiveFramePtr(*WWWFrameList);
    while (WWWFramePtr != NULL && strncasecmp(frameID,WWWFramePtr->header.frameId,4) != 0){
        ListFramePtr_setNextActive(WWWFrameList);
        WWWFramePtr = ListFramePtr_getActiveFramePtr(*WWWFrameList);
    }
    if(WWWFramePtr != NULL) {
        PrintFrame_WWWF(*WWWFramePtr,version);
        printf("\n\nFRAME ALREADY EXIST. Want to delete this frame? (y/n): ");
        do{
            option = getchar();
            cleanInputBuffer();
        }while(option != 'y' && option != 'Y' && option != 'n' && option != 'N');
        if(option == 'n' || option == 'N') { return 0;}
        deletedSize = FramesV2_getFrameSize(version,WWWFramePtr->header)+10;
        FramesV2_freeWWWF(&WWWFramePtr);
        ListFramePtr_deleteActive(WWWFrameList);
        incrementedSize -= deletedSize;
    }
    WWWFramePtr = GetFrame_WWWF(frameID,version);
    ListFramePtr_insertLast(WWWFrameList,WWWFramePtr);
    incrementedSize += FramesV2_getFrameSize(version,WWWFramePtr->header);
    PrintFrame_WWWF(*WWWFramePtr,version);
    return (incrementedSize + 10);
}