#include "PrintFrameManager.h"

void PrintFrame_PrintTXTFrames(ListFramePtr *TXTFrameList){
  ID3v2TXTFrameType *TXTFrame;
  ListFramePtr_setFirstActive(TXTFrameList);
  while(TXTFrameList->active != NULL){
    TXTFrame = ListFramePtr_getActiveFramePtr(*TXTFrameList);
    FramesV2_printTXTF(*TXTFrame);
    ListFramePtr_setNextActive(TXTFrameList);
  }
}

void PrintFrame_PrintCOMMFrames(COMMFrameList *COMMFrameList){
  ID3v2COMMFrameType COMMFrame;
  ListCOMM_setFirstActive(COMMFrameList);
  while(COMMFrameList->active != NULL){
    COMMFrame = ListCOMM_getActive(*COMMFrameList);
    FramesV2_printCOMM(COMMFrame);
    ListCOMM_setNextActive(COMMFrameList);
  }
}

void PrintFrame_PrintPRIVFrames(PRIVFrameList *PRIVFrameList){
  ID3v2PRIVFrameType PRIVFrame;
  ListPRIV_setFirstActive(PRIVFrameList);
  while(PRIVFrameList->active != NULL){
    PRIVFrame = ListPRIV_getActive(*PRIVFrameList);
    FramesV2_printPRIV(PRIVFrame);
    ListPRIV_setNextActive(PRIVFrameList);
  }
}