#include "SizeFrameManager.h"
#include "ID3v2Frames.h"
#include "ListFramePtr.h"

size_t SizeFrame_getFrameListSize(ID3TagType *ID3Tag){
    size_t tagSizeOfStruct = 10; //header
    ID3v2TXTFrameType *TXTPtr;
    ID3v2APICFrameType *APICFrame;
    ID3v2WWWFrameType *WWWFrame;
    ID3v2WXXXFrameType *WXXXFrame;
    ID3v2COMMFrameType *COMMFrame;
    ID3v2PRIVFrameType *PRIVFrame;
    ID3v2POPMFrameType *POPMFrame; 

    // ID3v2DefaultFrameType defaultPtr;

    ListFramePtr_setFirstActive(&ID3Tag->TXTFrameList);
    while(ID3Tag->TXTFrameList.active != NULL){
        TXTPtr = ListFramePtr_getActiveFramePtr(ID3Tag->TXTFrameList);
        tagSizeOfStruct += FramesV2_getFrameSize(ID3Tag->header.version[0],TXTPtr->header) + 10;
        ListFramePtr_setNextActive(&ID3Tag->TXTFrameList);
    }
    ListFramePtr_setFirstActive(&ID3Tag->COMMFrameList);
    while(ID3Tag->COMMFrameList.active != NULL){
        COMMFrame = ListFramePtr_getActiveFramePtr(ID3Tag->COMMFrameList);
        tagSizeOfStruct += FramesV2_getFrameSize(ID3Tag->header.version[0],COMMFrame->header) + 10;
        ListFramePtr_setNextActive(&ID3Tag->COMMFrameList);
    }
    ListFramePtr_setFirstActive(&ID3Tag->APICFrameList);
    while(ID3Tag->APICFrameList.active != NULL){
        APICFrame = ListFramePtr_getActiveFramePtr(ID3Tag->APICFrameList);
        tagSizeOfStruct +=  FramesV2_getFrameSize(ID3Tag->header.version[0],APICFrame->header) + 10;
        ListFramePtr_setNextActive(&ID3Tag->APICFrameList);
    }
    ListFramePtr_setFirstActive(&ID3Tag->PRIVFrameList);
    while(ID3Tag->PRIVFrameList.active != NULL){
        PRIVFrame = ListFramePtr_getActiveFramePtr(ID3Tag->PRIVFrameList);
        tagSizeOfStruct += FramesV2_getFrameSize(ID3Tag->header.version[0],PRIVFrame->header) + 10;
        ListFramePtr_setNextActive(&ID3Tag->PRIVFrameList);
    }
    ListFramePtr_setFirstActive(&ID3Tag->WWWFrameList);
    while(ID3Tag->WWWFrameList.active != NULL){
        WWWFrame = ListFramePtr_getActiveFramePtr(ID3Tag->WWWFrameList);
        tagSizeOfStruct += FramesV2_getFrameSize(ID3Tag->header.version[0],WWWFrame->header) + 10;
        ListFramePtr_setNextActive(&ID3Tag->WWWFrameList);
    }
    ListFramePtr_setFirstActive(&ID3Tag->WXXXFrameList);
    while(ID3Tag->WXXXFrameList.active != NULL){
        WXXXFrame = ListFramePtr_getActiveFramePtr(ID3Tag->WXXXFrameList);
        tagSizeOfStruct += FramesV2_getFrameSize(ID3Tag->header.version[0],WXXXFrame->header) + 10;
        ListFramePtr_setNextActive(&ID3Tag->WXXXFrameList);
    }
    ListFramePtr_setFirstActive(&ID3Tag->POPMFrameList);
    while(ID3Tag->POPMFrameList.active != NULL){
        POPMFrame = ListFramePtr_getActiveFramePtr(ID3Tag->POPMFrameList);
        tagSizeOfStruct += FramesV2_getFrameSize(ID3Tag->header.version[0],POPMFrame->header) + 10;
        ListFramePtr_setNextActive(&ID3Tag->POPMFrameList);
    }
    return tagSizeOfStruct;
}