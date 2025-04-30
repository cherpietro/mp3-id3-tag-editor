#include "PrintFrameManager.h"

void PrintFrame_PrintTXTFrames(ListFramePtr *TXTFrameList){
  ID3v2TXTFrameType *TXTFrame;
  ListFramePtr_setFirstActive(TXTFrameList);
  while(TXTFrameList->active != NULL){
    TXTFrame = (ID3v2TXTFrameType *)ListFramePtr_getActiveFramePtr(*TXTFrameList);
    FramesV2_printTXTF(*TXTFrame);
    ListFramePtr_setNextActive(TXTFrameList);
  }
}

void PrintFrame_PrintCOMMFrames(ListFramePtr *COMMFrameList){
  ID3v2COMMFrameType *COMMFrame;
  ListFramePtr_setFirstActive(COMMFrameList);
  while(COMMFrameList->active != NULL){
    COMMFrame = (ID3v2COMMFrameType *)ListFramePtr_getActiveFramePtr(*COMMFrameList);
    FramesV2_printCOMM(*COMMFrame);
    ListFramePtr_setNextActive(COMMFrameList);
  }
}

void PrintFrame_PrintPRIVFrames(ListFramePtr *PRIVFrameList){
  ID3v2PRIVFrameType *PRIVFrame;
  ListFramePtr_setFirstActive(PRIVFrameList);
  while(PRIVFrameList->active != NULL){
    PRIVFrame = (ID3v2PRIVFrameType *) ListFramePtr_getActiveFramePtr(*PRIVFrameList);
    FramesV2_printPRIV(*PRIVFrame);
    ListFramePtr_setNextActive(PRIVFrameList);
  }
}