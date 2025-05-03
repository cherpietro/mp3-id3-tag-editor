#include "AddFrameManager.h"
#include "PrintFrameManager.h"
#include "ID3v2Frames.h"
#include <string.h>


#define ADD_FRAME(FrameList,FrameType,FramePtr,FrameGetFunct,FramePrintFunct)\
    FrameType *FramePtr = FrameGetFunct(version);\
    if(FramePtr == NULL)return 0;\
    ListFramePtr_insertLast(FrameList,FramePtr);\
    FramePrintFunct(*FramePtr);\
    return (FramesV2_getFrameSize(version,FramePtr->header) +10);

static void cleanInputBuffer(){
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
}

int AddFrame_addTXXX(ListFramePtr *TXTFrameList,int version){
    ADD_FRAME(TXTFrameList,ID3v2TXTFrameType,TXTFramePtr,FramesV2_getTXXX,PrintFrame_TXTF);
}

int AddFrame_addWXXX(ListFramePtr *WXXXFrameList,int version){
    ADD_FRAME(WXXXFrameList,ID3v2WXXXFrameType,WXXXFramePtr,FramesV2_getWXXX,PrintFrame_WXXX);
}

int AddFrame_addAPIC(ListFramePtr *APICFrameList,int version){
    ADD_FRAME(APICFrameList,ID3v2APICFrameType,APICFramePtr,FramesV2_getAPIC,PrintFrame_APIC);
}

int AddFrame_addCOMM(ListFramePtr *COMMFrameList,int version){
    ADD_FRAME(COMMFrameList,ID3v2COMMFrameType,COMMFramePtr,FramesV2_getCOMM,PrintFrame_COMM);
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
        PrintFrame_TXTF(*TXTFramePtr);
        printf("\n\nFRAME ALREADY EXIST. Want to delete this frame? (y/n): ");
        option = getchar();
        cleanInputBuffer();
        if(option == 'n') { return 0;}
        deletedSize = FramesV2_getFrameSize(version,TXTFramePtr->header)+10;
        FramesV2_freeTXTF(&TXTFramePtr);
        ListFramePtr_deleteActive(TXTFrameList);
        incrementedSize -= deletedSize;
    }
    TXTFramePtr = FramesV2_getTXTF(frameID,version);
    ListFramePtr_insertLast(TXTFrameList,TXTFramePtr);
    incrementedSize += FramesV2_getFrameSize(version,TXTFramePtr->header);
    PrintFrame_TXTF(*TXTFramePtr);
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
        PrintFrame_WWWF(*WWWFramePtr);
        printf("\n\nFRAME ALREADY EXIST. Want to delete this frame? (y/n): ");
        option = getchar();
        cleanInputBuffer();
        if(option == 'n') { return 0;}
        deletedSize = FramesV2_getFrameSize(version,WWWFramePtr->header)+10;
        FramesV2_freeWWWF(&WWWFramePtr);
        ListFramePtr_deleteActive(WWWFrameList);
        incrementedSize -= deletedSize;
    }
    WWWFramePtr = FramesV2_getWWWF(frameID,version);
    ListFramePtr_insertLast(WWWFrameList,WWWFramePtr);
    incrementedSize += FramesV2_getFrameSize(version,WWWFramePtr->header);
    PrintFrame_WWWF(*WWWFramePtr);
    return (incrementedSize + 10);
}