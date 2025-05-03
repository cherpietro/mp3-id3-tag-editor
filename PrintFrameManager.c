#include "PrintFrameManager.h"
#include <string.h>

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

void PrintFrame_PrintAPICFrames(ListFramePtr *APICFrameList){
  ID3v2APICFrameType *APICFrame;
  ListFramePtr_setFirstActive(APICFrameList);
  while(APICFrameList->active != NULL){
    APICFrame = (ID3v2APICFrameType *) ListFramePtr_getActiveFramePtr(*APICFrameList);
    FramesV2_printAPIC(*APICFrame);
    FramesV2_saveAPICImage(*APICFrame);
    ListFramePtr_setNextActive(APICFrameList);
  }
}

void PrintFrame_PrintPOPMFrames(ListFramePtr *POPMFrameList){
  ID3v2POPMFrameType *POPMFrame;
  ListFramePtr_setFirstActive(POPMFrameList);
  while(POPMFrameList->active != NULL){
    POPMFrame = (ID3v2POPMFrameType *) ListFramePtr_getActiveFramePtr(*POPMFrameList);
    FramesV2_printPOPM(*POPMFrame);
    ListFramePtr_setNextActive(POPMFrameList);
  }
}

void printTag(ID3TagType *ID3Tag){
  HeaderV2_printTagHeader(ID3Tag->header);
  PrintFrame_PrintTXTFrames(&ID3Tag->TXTFrameList);  
  PrintFrame_PrintCOMMFrames(&ID3Tag->COMMFrameList);
  PrintFrame_PrintPRIVFrames(&ID3Tag->PRIVFrameList);
  PrintFrame_PrintAPICFrames(&ID3Tag->APICFrameList);
  // if(ID3Tag->POPM != NULL) FramesV2_printPOPM(*ID3Tag->POPM);
  PrintFrame_PrintPOPMFrames(&ID3Tag->POPMFrameList);
  if(ID3Tag->MCDI != NULL) FramesV2_printMDCI(*ID3Tag->MCDI);
}

void PrintFrame_printFrame(ID3TagType *ID3Tag, char *frameID){
  if(strncasecmp(frameID,"MCDI",4)==0){ if(ID3Tag->MCDI != NULL) FramesV2_printMDCI(*ID3Tag->MCDI);}
  // else if(strncasecmp(frameID,"SYTC",4)==0){ if(ID3Tag->SYTC != NULL) FramesV2_printSYTC(*ID3Tag->SYTC);}
  // else if(strncasecmp(frameID,"IPLS",4)==0){ if(ID3Tag->IPLS != NULL) FramesV2_printIPLS(*ID3Tag->IPLS);}
  // else if(strncasecmp(frameID,"USER",4)==0){ if(ID3Tag->USER != NULL) FramesV2_printUSER(*ID3Tag->USER);}
  // else if(strncasecmp(frameID,"OWNE",4)==0){ if(ID3Tag->OWNE != NULL) FramesV2_printOWNE(*ID3Tag->OWNE);}
  // else if(strncasecmp(frameID,"PCNT",4)==0){ if(ID3Tag->PCNT != NULL) FramesV2_printPCNT(*ID3Tag->PCNT);}

  else if(strncasecmp(frameID,"RVRB",4)==0);
  else if(strncasecmp(frameID,"EQUA",4)==0);
  else if(strncasecmp(frameID,"MLLT",4)==0);
  else if(strncasecmp(frameID,"ETCO",4)==0);
  else if(strncasecmp(frameID,"RVAD",4)==0);

  //LISTS
  else if(strncasecmp(frameID,"TXXX",4)==0){
    ListFramePtr_setFirstActive(&ID3Tag->TXTFrameList);
    ID3v2TXTFrameType * TXTFramePtr = ListFramePtr_getActiveFramePtr(ID3Tag->TXTFrameList);
    while (TXTFramePtr != NULL){
      if(strncasecmp(frameID,TXTFramePtr->header.frameId,4) == 0) FramesV2_printTXTF(*TXTFramePtr);
      ListFramePtr_setNextActive(&ID3Tag->TXTFrameList);
      TXTFramePtr = ListFramePtr_getActiveFramePtr(ID3Tag->TXTFrameList);
    }
  }
  else if(strncasecmp(frameID,"T",1)==0){
    ListFramePtr_setFirstActive(&ID3Tag->TXTFrameList);
    ID3v2TXTFrameType * TXTFramePtr = ListFramePtr_getActiveFramePtr(ID3Tag->TXTFrameList);
    while (TXTFramePtr != NULL && strncasecmp(frameID,TXTFramePtr->header.frameId,4) != 0){
      ListFramePtr_setNextActive(&ID3Tag->TXTFrameList);
      TXTFramePtr = ListFramePtr_getActiveFramePtr(ID3Tag->TXTFrameList);
    }
    if(TXTFramePtr != NULL) FramesV2_printTXTF(*TXTFramePtr);
  }
  else if(strncasecmp(frameID,"WXXX",4)==0){
    ListFramePtr_setFirstActive(&ID3Tag->WXXXFrameList);
    ID3v2WXXXFrameType * WXXXFramePtr = ListFramePtr_getActiveFramePtr(ID3Tag->WXXXFrameList);
    while (WXXXFramePtr != NULL){
      if(strncasecmp(frameID,WXXXFramePtr->header.frameId,4) == 0) FramesV2_printWXXX(*WXXXFramePtr);
      ListFramePtr_setNextActive(&ID3Tag->WXXXFrameList);
      WXXXFramePtr = ListFramePtr_getActiveFramePtr(ID3Tag->WXXXFrameList);
    }
  }
  else if(strncasecmp(frameID,"w",1)==0){
    ListFramePtr_setFirstActive(&ID3Tag->WWWFrameList);
    ID3v2WWWFrameType * WWWFramePtr = ListFramePtr_getActiveFramePtr(ID3Tag->WWWFrameList);
    while (WWWFramePtr != NULL && strncasecmp(frameID,WWWFramePtr->header.frameId,4) != 0){
      ListFramePtr_setNextActive(&ID3Tag->WWWFrameList);
      WWWFramePtr = ListFramePtr_getActiveFramePtr(ID3Tag->WWWFrameList);
    }
    if(WWWFramePtr != NULL) FramesV2_printWWWF(*WWWFramePtr);
  }
  else if(strncasecmp(frameID,"PRIV",4)==0){
    ListFramePtr_setFirstActive(&ID3Tag->PRIVFrameList);
    ID3v2PRIVFrameType * PRIVFramePtr = ListFramePtr_getActiveFramePtr(ID3Tag->PRIVFrameList);
    while (PRIVFramePtr != NULL){
      FramesV2_printPRIV(*PRIVFramePtr);
      ListFramePtr_setNextActive(&ID3Tag->PRIVFrameList);
      PRIVFramePtr = ListFramePtr_getActiveFramePtr(ID3Tag->PRIVFrameList);
    }
  }
  else if(strncasecmp(frameID,"COMM",4)==0){
    ListFramePtr_setFirstActive(&ID3Tag->COMMFrameList);
    ID3v2COMMFrameType * COMMFramePtr = ListFramePtr_getActiveFramePtr(ID3Tag->COMMFrameList);
    while (COMMFramePtr != NULL){
      FramesV2_printCOMM(*COMMFramePtr);
      ListFramePtr_setNextActive(&ID3Tag->COMMFrameList);
      COMMFramePtr = ListFramePtr_getActiveFramePtr(ID3Tag->COMMFrameList);
    }
  }
  else if(strncasecmp(frameID,"APIC",4)==0){
    ListFramePtr_setFirstActive(&ID3Tag->APICFrameList);
    ID3v2APICFrameType * APICFramePtr = ListFramePtr_getActiveFramePtr(ID3Tag->APICFrameList);
    while (APICFramePtr != NULL){
      FramesV2_printAPIC(*APICFramePtr);
      ListFramePtr_setNextActive(&ID3Tag->APICFrameList);
      APICFramePtr = ListFramePtr_getActiveFramePtr(ID3Tag->APICFrameList);
    }
  }
  else if(strncasecmp(frameID,"POPM",4)==0){ 
    ListFramePtr_setFirstActive(&ID3Tag->POPMFrameList);
    ID3v2POPMFrameType * POPMFramePtr = ListFramePtr_getActiveFramePtr(ID3Tag->POPMFrameList);
    while (POPMFramePtr != NULL){
      FramesV2_printPOPM(*POPMFramePtr);
      ListFramePtr_setNextActive(&ID3Tag->POPMFrameList);
      POPMFramePtr = ListFramePtr_getActiveFramePtr(ID3Tag->POPMFrameList);
    }
    // if(ID3Tag->POPM != NULL) FramesV2_printPOPM(*ID3Tag->POPM);
  }
  else printf("No frame in tag"); //Because of if anidation doesn't work in all tags
}

void PrintFrame_listFrames(ID3TagType *ID3Tag){
  if(ID3Tag->MCDI != NULL) printf("FramgeID: MCDI\n");
  // if(ID3Tag->IPLS != NULL) printf("FramgeID: IPLS\n");
  // if(ID3Tag->SYTC != NULL) printf("FramgeID: SYTC\n");
  // if(ID3Tag->USER != NULL) printf("FramgeID: USER\n");
  // if(ID3Tag->OWNE != NULL) printf("FramgeID: OWNE\n");
  // if(ID3Tag->PCNT != NULL) printf("FramgeID: PCNT\n");

  // if(ID3Tag->POPM != NULL) printf("FramgeID: POPM\n");
  ListFramePtr_setFirstActive(&(*ID3Tag).POPMFrameList);
  int POPMCount = 0;
  while(ID3Tag->APICFrameList.active != NULL){
    POPMCount += 1;
    ListFramePtr_setNextActive(&ID3Tag->POPMFrameList);
  }
  if(POPMCount != 0) printf("FramgeID: POPM (%d frames)\n",POPMCount);


  ListFramePtr_setFirstActive(&(*ID3Tag).APICFrameList);
  int APICCount = 0;
  while(ID3Tag->APICFrameList.active != NULL){
    APICCount += 1;
    ListFramePtr_setNextActive(&ID3Tag->APICFrameList);
  }
  if(APICCount != 0) printf("FramgeID: APIC (%d frames)\n",APICCount);

  ListFramePtr_setFirstActive(&(*ID3Tag).COMMFrameList);
  int COMMCount = 0;
  while(ID3Tag->COMMFrameList.active != NULL){
    COMMCount += 1;
    ListFramePtr_setNextActive(&ID3Tag->COMMFrameList);
  }
  if(COMMCount != 0) printf("FramgeID: COMM (%d frames)\n",COMMCount);

  int TXTFCount = 0;
  ListFramePtr_setFirstActive(&ID3Tag->TXTFrameList);
  ID3v2TXTFrameType * TXTFramePtr = ListFramePtr_getActiveFramePtr(ID3Tag->TXTFrameList);
  while (TXTFramePtr != NULL){
    if(strncasecmp("TXXX",TXTFramePtr->header.frameId,4) == 0) TXTFCount += 1;
    else printf("FramgeID: %s\n",TXTFramePtr->header.frameId);
    ListFramePtr_setNextActive(&ID3Tag->TXTFrameList);
    TXTFramePtr = ListFramePtr_getActiveFramePtr(ID3Tag->TXTFrameList);
  }
  if(TXTFCount != 0) printf("FramgeID: TXXX (%d frames)\n",TXTFCount);
  
  int WXXXCount = 0;
  ListFramePtr_setFirstActive(&ID3Tag->WXXXFrameList);
  ID3v2WXXXFrameType * WXXXFramePtr = ListFramePtr_getActiveFramePtr(ID3Tag->WXXXFrameList);
  while (WXXXFramePtr != NULL){
    if(strncasecmp("WXXX",WXXXFramePtr->header.frameId,4) == 0) WXXXCount += 1;
    ListFramePtr_setNextActive(&ID3Tag->WXXXFrameList);
    WXXXFramePtr = ListFramePtr_getActiveFramePtr(ID3Tag->WXXXFrameList);
  }
  if(WXXXCount != 0) printf("FramgeID: WXXX (%d frames)\n",WXXXCount);

  ListFramePtr_setFirstActive(&ID3Tag->WWWFrameList);
  ID3v2WWWFrameType * WWWFramePtr = ListFramePtr_getActiveFramePtr(ID3Tag->WWWFrameList);
  while (WWWFramePtr != NULL){
    printf("FramgeID: %s\n",WWWFramePtr->header.frameId);
    ListFramePtr_setNextActive(&ID3Tag->WWWFrameList);
    WWWFramePtr = ListFramePtr_getActiveFramePtr(ID3Tag->WWWFrameList);
  }
  
  int PRIVCount = 0;
  ListFramePtr_setFirstActive(&(*ID3Tag).PRIVFrameList);
  while(ID3Tag->PRIVFrameList.active != NULL){
    PRIVCount += 1;
    ListFramePtr_setNextActive(&ID3Tag->PRIVFrameList);
  }
  if(PRIVCount != 0) printf("FramgeID: PRIV (%d frames)\n",PRIVCount);
}
