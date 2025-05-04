#include "AddFrameManager.h"
#include "PrintFrameManager.h"
#include "GetFrameManager.h"
#include "../ID3v2Frames.h"
#include <string.h>

#define ADD_FRAME_LIST(FrameList,FrameType,FramePtr,FrameGetFunct,FramePrintFunct)\
    FrameType *FramePtr = FrameGetFunct(version);\
    if(FramePtr == NULL)return 0;\
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
    ADD_FRAME_LIST(TXTFrameList,ID3v2TXTFrameType,TXTFramePtr,GetFrame_TXXX,PrintFrame_TXTF);
}

int AddFrame_addWXXX(ListFramePtr *WXXXFrameList,int version){
    ADD_FRAME_LIST(WXXXFrameList,ID3v2WXXXFrameType,WXXXFramePtr,GetFrame_WXXX,PrintFrame_WXXX);
}

int AddFrame_addAPIC(ListFramePtr *APICFrameList,int version){
    ADD_FRAME_LIST(APICFrameList,ID3v2APICFrameType,APICFramePtr,GetFrame_APIC,PrintFrame_APIC);
}

int AddFrame_addCOMM(ListFramePtr *COMMFrameList,int version){
    ADD_FRAME_LIST(COMMFrameList,ID3v2COMMFrameType,COMMFramePtr,GetFrame_COMM,PrintFrame_COMM);
}
int AddFrame_addPOPM(ListFramePtr *POPMFrameList,int version){
    ADD_FRAME_LIST(POPMFrameList,ID3v2POPMFrameType,POPMFramePtr,GetFrame_POPM,PrintFrame_POPM);
}
int AddFrame_addPCNT(ID3v2PCNTFrameType **PCNT,int version){
    ADD_FRAME(PCNT,GetFrame_PCNT,PrintFrame_PCNT);
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
        option = getchar();
        cleanInputBuffer();
        if(option == 'n') { return 0;}
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
        option = getchar();
        cleanInputBuffer();
        if(option == 'n') { return 0;}
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