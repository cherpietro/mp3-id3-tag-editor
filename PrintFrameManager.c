#include "PrintFrameManager.h"
#include <string.h>

#define PRINT_TEXTSTR(frame,txtStr)\
    for (int i = 0; i < (int)strlen(frame.txtStr.string); i++) {\
        if(frame.txtStr.string[i] == '\0') printf(" ");\
        else putchar(frame.txtStr.string[i]);\
    }\
    printf("\n");

#define PRINT_LIST_FRAME(FrameLst,FramePtr,FrameType,printFunct)\
FrameType *FramePtr;\
  ListFramePtr_setFirstActive(&FrameLst);\
  while(FrameLst.active != NULL){\
    FramePtr = (FrameType *)ListFramePtr_getActiveFramePtr(FrameLst);\
    printFunct(*FramePtr);\
    ListFramePtr_setNextActive(&FrameLst);\
  }

  //Unused
void printTag(ID3TagType *ID3Tag){
  HeaderV2_printTagHeader(ID3Tag->header);
  PRINT_LIST_FRAME(ID3Tag->TXTFrameList,TXTFrame,ID3v2TXTFrameType,FramesV2_printTXTF);  
  PRINT_LIST_FRAME(ID3Tag->COMMFrameList,COMMFrame,ID3v2COMMFrameType,FramesV2_printCOMM); 
  PRINT_LIST_FRAME(ID3Tag->PRIVFrameList,PRIVFrame,ID3v2PRIVFrameType,FramesV2_printPRIV); 
  PRINT_LIST_FRAME(ID3Tag->APICFrameList,APICFrame,ID3v2APICFrameType,FramesV2_printAPIC); 
  PRINT_LIST_FRAME(ID3Tag->POPMFrameList,POPMFrame,ID3v2POPMFrameType,FramesV2_printPOPM); 
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

void FramesV2_printAPIC(ID3v2APICFrameType APIC){
  printf("\n----FRAME----\n");
  printf("Frame ID: %s\n",APIC.header.frameId);
  printf("Flags: %u %u\n",APIC.header.flags[0],APIC.header.flags[1]);
  printf("TextEncoding: %d\n",APIC.textEncoding);
  printf("MIME type: %s\n",APIC.mimeType.string);
  printf("apicframe.pictureType: %u\n",APIC.pictureType);
  printf("apicframe.description: %s\n",APIC.description.string);
  printf("apicframe.imageSize: %ld\n",APIC.imageDataSize);
  FramesV2_saveAPICImage(APIC);
}

void FramesV2_printTXTF(ID3v2TXTFrameType frame){
  printf("\n----FRAME----\n");
  printf("Frame ID: %s\n",frame.header.frameId);
  printf("Flags: %u %u\n",frame.header.flags[0],frame.header.flags[1]);
  printf("TextEncoding: %d\n",frame.textEncoding);
  if(strncasecmp("TXXX",frame.header.frameId,4) == 0){
      printf("Description: ");
      for (int i = 0; i < (int)strlen(frame.content.string); i++) {
          if(frame.content.string[i] == '\0') printf(" ");
          else putchar(frame.content.string[i]);
      }
      printf("\n");
      printf("Content: ");
      for (int i = strlen(frame.content.string)+1; i < (int)frame.content.size; i++) {
          if(frame.content.string[i] == '\0') printf(" ");
          else putchar(frame.content.string[i]);
      }
      printf("\n");
  }
  else{
      printf("Content: ");
      for (int i = 0; i < (int)frame.content.size; i++) {
          if(frame.content.string[i] == '\0') printf(" ");
          else putchar(frame.content.string[i]);
      }
      printf("\n");
  }
}

void FramesV2_printCOMM(ID3v2COMMFrameType COMM){
  printf("\n----FRAME----\n");
  printf("Frame ID: %s\n",COMM.header.frameId);
  printf("Flags: %u %u\n",COMM.header.flags[0],COMM.header.flags[1]);
  printf("TextEncoding: %d\n",COMM.textEncoding);
  printf("Language: %s\n",COMM.language);
  printf("Descript: ");
  PRINT_TEXTSTR(COMM,contentDescript);
  printf("Text: ");
  PRINT_TEXTSTR(COMM,actualText);
}

void FramesV2_printPRIV(ID3v2PRIVFrameType PRIV){
  printf("\n----FRAME----\n");
  printf("Frame ID: %s\n",PRIV.header.frameId);
  printf("Flags: %u %u\n",PRIV.header.flags[0],PRIV.header.flags[1]);
  printf("Owner: ");
  PRINT_TEXTSTR(PRIV,owner);
}

void FramesV2_printMDCI(ID3v2MCDIFrameType MDCI){
  printf("\n----FRAME----\n");
  printf("Frame ID: %s\n",MDCI.header.frameId);
  printf("Flags: %u %u\n",MDCI.header.flags[0],MDCI.header.flags[1]);
}

void FramesV2_printPOPM(ID3v2POPMFrameType POPM){
  printf("\n----FRAME----\n");
  printf("Frame ID: %s\n",POPM.header.frameId);
  printf("Flags: %u %u\n",POPM.header.flags[0],POPM.header.flags[1]);
  printf("User email: %s\n",POPM.userEmail.string);
  printf("Ratin: %d\n",POPM.rating);
}

void FramesV2_printDefaultFrame(ID3v2DefaultFrameType DefaultFrame){
  printf("\n----FRAME----\n");
  printf("Frame ID: %s\n",DefaultFrame.header.frameId);
  printf("Flags: %u %u\n",DefaultFrame.header.flags[0],DefaultFrame.header.flags[1]);
}

void FramesV2_printWWWF(ID3v2WWWFrameType WWWF){
  printf("\n----FRAME----\n");
  printf("Frame ID: %s\n",WWWF.header.frameId);
  printf("Flags: %u %u\n",WWWF.header.flags[0],WWWF.header.flags[1]);
  printf("URL: ");
  PRINT_TEXTSTR(WWWF,url);
}

void FramesV2_printWXXX(ID3v2WXXXFrameType WXXX){
  printf("\n----FRAME----\n");
  printf("Frame ID: %s\n",WXXX.header.frameId);
  printf("Flags: %u %u\n",WXXX.header.flags[0],WXXX.header.flags[1]);
  printf("TextEncoding: %d\n",WXXX.textEncoding);
  printf("Description: ");
  PRINT_TEXTSTR(WXXX,description);
  printf("url: ");
}

void FramesV2_saveAPICImage(ID3v2APICFrameType APIC){
  FILE *imageFile = fopen("cover.jpg", "wb");
  fwrite(APIC.imageData, 1, APIC.imageDataSize, imageFile);
  fclose(imageFile);
  printf("File saved in ./cover.jpg\n");
}
