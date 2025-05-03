#include "AddFrameManager.h"
#include "ID3v2Frames.h"
#include <string.h>

static void cleanInputBuffer(){
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
}

int AddFrame_addTXXX(ListFramePtr *TXTFrameList,int version){
    ID3v2TXTFrameType *TXTFramePtr = FramesV2_getTXXX();
    ListFramePtr_insertLast(TXTFrameList,TXTFramePtr);
    FramesV2_updateFrameSize(version,&TXTFramePtr->header,TXTFramePtr->content.size);
    FramesV2_printTXTF(*TXTFramePtr);
    return (FramesV2_getFrameSize(version,TXTFramePtr->header) +10);
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
        FramesV2_printTXTF(*TXTFramePtr);
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
    FramesV2_printTXTF(*TXTFramePtr);
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
        FramesV2_printWWWF(*WWWFramePtr);
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
    FramesV2_printWWWF(*WWWFramePtr);
    return (incrementedSize + 10);
}

int AddFrame_addAPIC(ListFramePtr *APICFrameList,int version){
    ID3v2APICFrameType *APICFramePtr = FramesV2_getAPIC(version);
    if(APICFramePtr == NULL)return 0;
    ListFramePtr_insertLast(APICFrameList,APICFramePtr);
    FramesV2_printAPIC(*APICFramePtr);
    return (FramesV2_getFrameSize(version,APICFramePtr->header) +10);
}