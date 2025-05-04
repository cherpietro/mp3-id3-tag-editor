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
    printFunct(*FramePtr,version);\
    ListFramePtr_setNextActive(&FrameLst);\
  }

  //Unused
void printTag(ID3TagType *ID3Tag){
  int version = ID3Tag->header.version[0];
  HeaderV2_printTagHeader(ID3Tag->header);
  PRINT_LIST_FRAME(ID3Tag->TXTFrameList,TXTFrame,ID3v2TXTFrameType,PrintFrame_TXTF);  
  PRINT_LIST_FRAME(ID3Tag->COMMFrameList,COMMFrame,ID3v2COMMFrameType,PrintFrame_COMM); 
  PRINT_LIST_FRAME(ID3Tag->PRIVFrameList,PRIVFrame,ID3v2PRIVFrameType,PrintFrame_PRIV); 
  PRINT_LIST_FRAME(ID3Tag->APICFrameList,APICFrame,ID3v2APICFrameType,PrintFrame_APIC); 
  PRINT_LIST_FRAME(ID3Tag->POPMFrameList,POPMFrame,ID3v2POPMFrameType,PrintFrame_POPM); 
  PRINT_LIST_FRAME(ID3Tag->WWWFrameList,WWWFrame,ID3v2WWWFrameType,PrintFrame_WWWF); 
  PRINT_LIST_FRAME(ID3Tag->WXXXFrameList,WXXXFrame,ID3v2WXXXFrameType,PrintFrame_WXXX); 
  if(ID3Tag->MCDI != NULL) PrintFrame_MDCI(*ID3Tag->MCDI,version);
}

void PrintFrame_printFrame(ID3TagType *ID3Tag, char *frameID){
  int version = ID3Tag->header.version[0];
  if(strncasecmp(frameID,"MCDI",4)==0){ if(ID3Tag->MCDI != NULL) PrintFrame_MDCI(*ID3Tag->MCDI,version);}
  // else if(strncasecmp(frameID,"SYTC",4)==0){ if(ID3Tag->SYTC != NULL) PrintFrame_SYTC(*ID3Tag->SYTC);}
  // else if(strncasecmp(frameID,"IPLS",4)==0){ if(ID3Tag->IPLS != NULL) PrintFrame_IPLS(*ID3Tag->IPLS);}
  // else if(strncasecmp(frameID,"USER",4)==0){ if(ID3Tag->USER != NULL) PrintFrame_USER(*ID3Tag->USER);}
  // else if(strncasecmp(frameID,"OWNE",4)==0){ if(ID3Tag->OWNE != NULL) PrintFrame_OWNE(*ID3Tag->OWNE);}
  // else if(strncasecmp(frameID,"PCNT",4)==0){ if(ID3Tag->PCNT != NULL) PrintFrame_PCNT(*ID3Tag->PCNT);}

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
      if(strncasecmp(frameID,TXTFramePtr->header.frameId,4) == 0) PrintFrame_TXTF(*TXTFramePtr,version);
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
    if(TXTFramePtr != NULL) PrintFrame_TXTF(*TXTFramePtr,version);
  }
  else if(strncasecmp(frameID,"WXXX",4)==0){
    ListFramePtr_setFirstActive(&ID3Tag->WXXXFrameList);
    ID3v2WXXXFrameType * WXXXFramePtr = ListFramePtr_getActiveFramePtr(ID3Tag->WXXXFrameList);
    while (WXXXFramePtr != NULL){
      if(strncasecmp(frameID,WXXXFramePtr->header.frameId,4) == 0) PrintFrame_WXXX(*WXXXFramePtr,version);
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
    if(WWWFramePtr != NULL) PrintFrame_WWWF(*WWWFramePtr,version);
  }
  else if(strncasecmp(frameID,"PRIV",4)==0){
    PRINT_LIST_FRAME(ID3Tag->PRIVFrameList,PRIVFrame,ID3v2PRIVFrameType,PrintFrame_PRIV);
  }
  else if(strncasecmp(frameID,"COMM",4)==0){
    PRINT_LIST_FRAME(ID3Tag->COMMFrameList,COMMFrame,ID3v2COMMFrameType,PrintFrame_COMM); 
  }
  else if(strncasecmp(frameID,"APIC",4)==0){
    PRINT_LIST_FRAME(ID3Tag->APICFrameList,APICFrame,ID3v2APICFrameType,PrintFrame_APIC); 
  
  }
  else if(strncasecmp(frameID,"POPM",4)==0){ 
    PRINT_LIST_FRAME(ID3Tag->POPMFrameList,POPMFrame,ID3v2POPMFrameType,PrintFrame_POPM); 
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

void PrintFrame_APIC(ID3v2APICFrameType APIC, int version){
  printf("\n----FRAME----\n");
  printf("Frame ID: %s\n",APIC.header.frameId);
  printf("Flags: %u %u\n",APIC.header.flags[0],APIC.header.flags[1]);
  size_t size = FramesV2_getFrameSize(version,APIC.header);
  printf("Size: %ld\n",size);
  printf("TextEncoding: %d\n",APIC.textEncoding);
  printf("MIME type: %s\n",APIC.mimeType.string);
  printf("apicframe.pictureType: %u\n",APIC.pictureType);
  printf("apicframe.description: %s\n",APIC.description.string);
  printf("apicframe.imageSize: %ld\n",APIC.imageDataSize);
  PrintFrame_saveAPICImage(APIC);
}

void PrintFrame_TXTF(ID3v2TXTFrameType frame,int version){
  printf("\n----FRAME----\n");
  printf("Frame ID: %s\n",frame.header.frameId);
  printf("Flags: %u %u\n",frame.header.flags[0],frame.header.flags[1]);
  size_t size = FramesV2_getFrameSize(version,frame.header);
  printf("Size: %ld\n",size);
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

void PrintFrame_COMM(ID3v2COMMFrameType COMM,int version){
  printf("\n----FRAME----\n");
  printf("Frame ID: %s\n",COMM.header.frameId);
  printf("Flags: %u %u\n",COMM.header.flags[0],COMM.header.flags[1]);
  size_t size = FramesV2_getFrameSize(version,COMM.header);
  printf("Size: %ld\n",size);
  printf("TextEncoding: %d\n",COMM.textEncoding);
  printf("Language: %s\n",COMM.language);
  printf("Descript: ");
  PRINT_TEXTSTR(COMM,contentDescript);
  printf("Text: ");
  PRINT_TEXTSTR(COMM,actualText);
}

void PrintFrame_PRIV(ID3v2PRIVFrameType PRIV,int version){
  printf("\n----FRAME----\n");
  printf("Frame ID: %s\n",PRIV.header.frameId);
  printf("Flags: %u %u\n",PRIV.header.flags[0],PRIV.header.flags[1]);
  size_t size = FramesV2_getFrameSize(version,PRIV.header);
  printf("Size: %ld\n",size);
  printf("Owner: ");
  PRINT_TEXTSTR(PRIV,owner);
}

void PrintFrame_MDCI(ID3v2MCDIFrameType MDCI,int version){
  printf("\n----FRAME----\n");
  printf("Frame ID: %s\n",MDCI.header.frameId);
  printf("Flags: %u %u\n",MDCI.header.flags[0],MDCI.header.flags[1]);
  size_t size = FramesV2_getFrameSize(version,MDCI.header);
  printf("Size: %ld\n",size);
}

static uint32_t char_array_to_uint32(const char arr[4]) {
  return ((uint32_t)(unsigned char)arr[0] << 24) |
         ((uint32_t)(unsigned char)arr[1] << 16) |
         ((uint32_t)(unsigned char)arr[2] << 8)  |
         ((uint32_t)(unsigned char)arr[3]);
}
void PrintFrame_POPM(ID3v2POPMFrameType POPM,int version){
  printf("\n----FRAME----\n");
  printf("Frame ID: %s\n",POPM.header.frameId);
  printf("Flags: %u %u\n",POPM.header.flags[0],POPM.header.flags[1]);
  size_t size = FramesV2_getFrameSize(version,POPM.header);
  printf("Size: %ld\n",size);
  printf("User email: %s\n",POPM.userEmail.string);
  printf("Ratin: %d\n",POPM.rating);
  printf("Counter: %d\n",char_array_to_uint32(POPM.counter));
}

void PrintFrame_DefaultFrame(ID3v2DefaultFrameType DefaultFrame,int version){
  printf("\n----FRAME----\n");
  printf("Frame ID: %s\n",DefaultFrame.header.frameId);
  printf("Flags: %u %u\n",DefaultFrame.header.flags[0],DefaultFrame.header.flags[1]);
  size_t size = FramesV2_getFrameSize(version,DefaultFrame.header);
  printf("Size: %ld\n",size);
}

void PrintFrame_WWWF(ID3v2WWWFrameType WWWF,int version){
  printf("\n----FRAME----\n");
  printf("Frame ID: %s\n",WWWF.header.frameId);
  printf("Flags: %u %u\n",WWWF.header.flags[0],WWWF.header.flags[1]);
  size_t size = FramesV2_getFrameSize(version,WWWF.header);
  printf("Size: %ld\n",size);
  printf("URL: ");
  PRINT_TEXTSTR(WWWF,url);
}

void PrintFrame_WXXX(ID3v2WXXXFrameType WXXX,int version){
  printf("\n----FRAME----\n");
  printf("Frame ID: %s\n",WXXX.header.frameId);
  printf("Flags: %u %u\n",WXXX.header.flags[0],WXXX.header.flags[1]);
  size_t size = FramesV2_getFrameSize(version,WXXX.header);
  printf("Size: %ld\n",size);
  printf("TextEncoding: %d\n",WXXX.textEncoding);
  printf("Description: ");
  PRINT_TEXTSTR(WXXX,description);
  printf("url: ");
}

void PrintFrame_saveAPICImage(ID3v2APICFrameType APIC){
  FILE *imageFile = fopen("cover.jpg", "wb");
  fwrite(APIC.imageData, 1, APIC.imageDataSize, imageFile);
  fclose(imageFile);
  printf("File saved in ./cover.jpg\n");
}
