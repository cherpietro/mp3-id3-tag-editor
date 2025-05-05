#include "SizeFrameManager.h"
#include "../ID3v2Frames.h"
#include "../ListFramePtr.h"

#define ADD_FRAME_SIZE_LIST(frameList, frameType)\
    ListFramePtr_setFirstActive(&ID3Tag->frameList);\
    while(ID3Tag->frameList.active != NULL){\
        frameType *frame = ListFramePtr_getActiveFramePtr(ID3Tag->frameList);\
        tagSizeOfStruct += FramesV2_getFrameSize(ID3Tag->header.version[0], frame->header) + 10;\
        ListFramePtr_setNextActive(&ID3Tag->frameList);\
    }
#define ADD_FRAME_SIZE(frame)\
    if(ID3Tag->frame != NULL){tagSizeOfStruct += FramesV2_getFrameSize(ID3Tag->header.version[0], ID3Tag->frame->header) + 10;}


size_t SizeFrame_getFrameListSize(ID3TagType *ID3Tag){
    size_t tagSizeOfStruct = 10; // header

    ADD_FRAME_SIZE_LIST(TXTFrameList, ID3v2TXTFrameType);
    ADD_FRAME_SIZE_LIST(TXXXFrameList, ID3v2TXXXFrameType);
    ADD_FRAME_SIZE_LIST(COMMFrameList, ID3v2COMMFrameType);
    ADD_FRAME_SIZE_LIST(APICFrameList, ID3v2APICFrameType);
    ADD_FRAME_SIZE_LIST(PRIVFrameList, ID3v2PRIVFrameType);
    ADD_FRAME_SIZE_LIST(WWWFrameList, ID3v2WWWFrameType);
    ADD_FRAME_SIZE_LIST(WXXXFrameList, ID3v2WXXXFrameType);
    ADD_FRAME_SIZE_LIST(POPMFrameList, ID3v2POPMFrameType);
    ADD_FRAME_SIZE(MCDI);
    ADD_FRAME_SIZE(PCNT);
    
    ADD_FRAME_SIZE_LIST(UFIDFrameList, ID3v2DefaultFrameType);
    ADD_FRAME_SIZE_LIST(USLTFrameList, ID3v2DefaultFrameType);
    ADD_FRAME_SIZE_LIST(SYLTFrameList, ID3v2DefaultFrameType);
    ADD_FRAME_SIZE_LIST(GEOBFrameList, ID3v2DefaultFrameType);
    ADD_FRAME_SIZE_LIST(LINKFrameList, ID3v2DefaultFrameType);
    ADD_FRAME_SIZE_LIST(AENCFrameList, ID3v2DefaultFrameType);
    ADD_FRAME_SIZE_LIST(ENCRFrameList, ID3v2DefaultFrameType);
    ADD_FRAME_SIZE_LIST(GRIDFrameList, ID3v2DefaultFrameType);

    ADD_FRAME_SIZE(IPLS);
    ADD_FRAME_SIZE(SYTC);
    ADD_FRAME_SIZE(USER);
    ADD_FRAME_SIZE(OWNE);

    ADD_FRAME_SIZE(COMR);
    ADD_FRAME_SIZE(POSS);
    ADD_FRAME_SIZE(RVRB);
    ADD_FRAME_SIZE(EQUA);
    ADD_FRAME_SIZE(MLLT);
    ADD_FRAME_SIZE(ETCO);
    ADD_FRAME_SIZE(RVAD);

    return tagSizeOfStruct;
}
