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

#define ENUMERATE_LIST_FRAME(ListFramePtr,frameID)\
  Count = 0;\
  ListFramePtr_setFirstActive(&(*ID3Tag).ListFramePtr);\
  while(ID3Tag->ListFramePtr.active != NULL){\
      Count += 1;\
      ListFramePtr_setNextActive(&ID3Tag->ListFramePtr);\
    }\
  if(Count != 0) {sprintf(buffer,"FramgeID: %s (%d frames)\n",frameID,Count);printf(buffer);}

#define ENUMERATE_LIST_SINGLE_FRAME(ListFramePtr,framePtr,frameType)\
  ListFramePtr_setFirstActive(&ID3Tag->ListFramePtr);\
  frameType * framePtr = ListFramePtr_getActiveFramePtr(ID3Tag->ListFramePtr);\
  while (framePtr != NULL){\
    printf("FramgeID: %s\n",framePtr->header.frameId);\
    ListFramePtr_setNextActive(&ID3Tag->ListFramePtr);\
    framePtr = ListFramePtr_getActiveFramePtr(ID3Tag->ListFramePtr);\
  }

void printTag(ID3TagType *ID3Tag){
  int version = ID3Tag->header.version[0];
  HeaderV2_printTagHeader(ID3Tag->header);
  PRINT_LIST_FRAME(ID3Tag->TXTFrameList,TXTFrame,ID3v2TXTFrameType,PrintFrame_TXTF);  
  PRINT_LIST_FRAME(ID3Tag->TXXXFrameList,TXXXFrame,ID3v2TXXXFrameType,PrintFrame_TXXX);  
  PRINT_LIST_FRAME(ID3Tag->COMMFrameList,COMMFrame,ID3v2COMMFrameType,PrintFrame_COMM); 
  PRINT_LIST_FRAME(ID3Tag->PRIVFrameList,PRIVFrame,ID3v2PRIVFrameType,PrintFrame_PRIV); 
  PRINT_LIST_FRAME(ID3Tag->APICFrameList,APICFrame,ID3v2APICFrameType,PrintFrame_APIC); 
  PRINT_LIST_FRAME(ID3Tag->POPMFrameList,POPMFrame,ID3v2POPMFrameType,PrintFrame_POPM); 
  PRINT_LIST_FRAME(ID3Tag->WWWFrameList,WWWFrame,ID3v2WWWFrameType,PrintFrame_WWWF); 
  PRINT_LIST_FRAME(ID3Tag->WXXXFrameList,WXXXFrame,ID3v2WXXXFrameType,PrintFrame_WXXX); 
  if(ID3Tag->MCDI != NULL) PrintFrame_DefaultFrame(*ID3Tag->MCDI,version);
  if(ID3Tag->PCNT != NULL) PrintFrame_PCNT(*ID3Tag->PCNT,version);

  PRINT_LIST_FRAME(ID3Tag->UFIDFrameList,UFIDFrame,ID3v2DefaultFrameType,PrintFrame_DefaultFrame); 
  PRINT_LIST_FRAME(ID3Tag->SYLTFrameList,SYLTFrame,ID3v2DefaultFrameType,PrintFrame_DefaultFrame); 
  PRINT_LIST_FRAME(ID3Tag->GEOBFrameList,GEOBFrame,ID3v2DefaultFrameType,PrintFrame_DefaultFrame); 
  PRINT_LIST_FRAME(ID3Tag->LINKFrameList,LINKFrame,ID3v2DefaultFrameType,PrintFrame_DefaultFrame); 
  PRINT_LIST_FRAME(ID3Tag->AENCFrameList,AENCFrame,ID3v2DefaultFrameType,PrintFrame_DefaultFrame); 
  PRINT_LIST_FRAME(ID3Tag->ENCRFrameList,ENCRFrame,ID3v2DefaultFrameType,PrintFrame_DefaultFrame); 
  PRINT_LIST_FRAME(ID3Tag->GRIDFrameList,GRIDFrame,ID3v2DefaultFrameType,PrintFrame_DefaultFrame); 
  
  if(ID3Tag->IPLS != NULL) PrintFrame_IPLS(*ID3Tag->IPLS,version);
  if(ID3Tag->SYTC != NULL) PrintFrame_DefaultFrame(*ID3Tag->SYTC,version);
  if(ID3Tag->USER != NULL) PrintFrame_USER(*ID3Tag->USER,version);
  if(ID3Tag->OWNE != NULL) PrintFrame_DefaultFrame(*ID3Tag->OWNE,version);
  
  if(ID3Tag->COMR != NULL) PrintFrame_DefaultFrame(*ID3Tag->COMR,version);
  if(ID3Tag->POSS != NULL) PrintFrame_DefaultFrame(*ID3Tag->POSS,version);
  if(ID3Tag->RVRB != NULL) PrintFrame_RVRB(*ID3Tag->RVRB,version);
  if(ID3Tag->EQUA != NULL) PrintFrame_DefaultFrame(*ID3Tag->EQUA,version);
  if(ID3Tag->MLLT != NULL) PrintFrame_DefaultFrame(*ID3Tag->MLLT,version);
  if(ID3Tag->ETCO != NULL) PrintFrame_DefaultFrame(*ID3Tag->ETCO,version);
  if(ID3Tag->RVAD != NULL) PrintFrame_DefaultFrame(*ID3Tag->RVAD,version);
}

void PrintFrame_printFrame(ID3TagType *ID3Tag, char *frameID){
  int version = ID3Tag->header.version[0];
  if(strncasecmp(frameID,"TXXX",4)==0){
    ListFramePtr_setFirstActive(&ID3Tag->TXXXFrameList);
    ID3v2TXXXFrameType * TXXXFramePtr = ListFramePtr_getActiveFramePtr(ID3Tag->TXXXFrameList);
    while (TXXXFramePtr != NULL){
      if(strncasecmp(frameID,TXXXFramePtr->header.frameId,4) == 0) PrintFrame_TXXX(*TXXXFramePtr,version);
      ListFramePtr_setNextActive(&ID3Tag->TXXXFrameList);
      TXXXFramePtr = ListFramePtr_getActiveFramePtr(ID3Tag->TXXXFrameList);
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
  else if(strncasecmp(frameID,"APIC",4)==0){ PRINT_LIST_FRAME(ID3Tag->APICFrameList,APICFrame,ID3v2APICFrameType,PrintFrame_APIC);}
  else if(strncasecmp(frameID,"W",1)==0){
    ListFramePtr_setFirstActive(&ID3Tag->WWWFrameList);
    ID3v2WWWFrameType * WWWFramePtr = ListFramePtr_getActiveFramePtr(ID3Tag->WWWFrameList);
    while (WWWFramePtr != NULL && strncasecmp(frameID,WWWFramePtr->header.frameId,4) != 0){
      ListFramePtr_setNextActive(&ID3Tag->WWWFrameList);
      WWWFramePtr = ListFramePtr_getActiveFramePtr(ID3Tag->WWWFrameList);
    }
    if(WWWFramePtr != NULL) PrintFrame_WWWF(*WWWFramePtr,version);
  }
  else if(strncasecmp(frameID,"WXXX",4)==0){
    ListFramePtr_setFirstActive(&ID3Tag->WXXXFrameList);
    ID3v2WXXXFrameType * WXXXFramePtr = ListFramePtr_getActiveFramePtr(ID3Tag->WXXXFrameList);
    while (WXXXFramePtr != NULL){
      PrintFrame_WXXX(*WXXXFramePtr,version);
      ListFramePtr_setNextActive(&ID3Tag->WXXXFrameList);
      WXXXFramePtr = ListFramePtr_getActiveFramePtr(ID3Tag->WXXXFrameList);
    }
  }
  else if(strncasecmp(frameID,"COMM",4)==0){ PRINT_LIST_FRAME(ID3Tag->COMMFrameList,COMMFrame,ID3v2COMMFrameType,PrintFrame_COMM);}
  else if(strncasecmp(frameID,"POPM",4)==0){ PRINT_LIST_FRAME(ID3Tag->POPMFrameList,POPMFrame,ID3v2POPMFrameType,PrintFrame_POPM);}
  else if(strncasecmp(frameID,"PCNT",4)==0){ if(ID3Tag->PCNT != NULL) PrintFrame_PCNT(*ID3Tag->PCNT,version);}
  else if(strncasecmp(frameID,"PRIV",4)==0){ PRINT_LIST_FRAME(ID3Tag->PRIVFrameList,PRIVFrame,ID3v2PRIVFrameType,PrintFrame_PRIV);}
  else if(strncasecmp(frameID,"MCDI",4)==0){ if(ID3Tag->MCDI != NULL) PrintFrame_DefaultFrame(*ID3Tag->MCDI,version);}
  
  else if(strncasecmp(frameID,"UFID",4)==0){ PRINT_LIST_FRAME(ID3Tag->UFIDFrameList,UFIDFrame,ID3v2DefaultFrameType,PrintFrame_DefaultFrame);}
  else if(strncasecmp(frameID,"USLT",4)==0){ PRINT_LIST_FRAME(ID3Tag->USLTFrameList,USLTFrame,ID3v2DefaultFrameType,PrintFrame_DefaultFrame);}
  else if(strncasecmp(frameID,"SYLT",4)==0){ PRINT_LIST_FRAME(ID3Tag->SYLTFrameList,SYLTFrame,ID3v2DefaultFrameType,PrintFrame_DefaultFrame);}
  else if(strncasecmp(frameID,"GEOB",4)==0){ PRINT_LIST_FRAME(ID3Tag->GEOBFrameList,GEOBFrame,ID3v2DefaultFrameType,PrintFrame_DefaultFrame);}
  else if(strncasecmp(frameID,"LINK",4)==0){ PRINT_LIST_FRAME(ID3Tag->LINKFrameList,LINKFrame,ID3v2DefaultFrameType,PrintFrame_DefaultFrame);}
  else if(strncasecmp(frameID,"AENC",4)==0){ PRINT_LIST_FRAME(ID3Tag->AENCFrameList,AENCFrame,ID3v2DefaultFrameType,PrintFrame_DefaultFrame);}
  else if(strncasecmp(frameID,"ENCR",4)==0){ PRINT_LIST_FRAME(ID3Tag->ENCRFrameList,ENCRFrame,ID3v2DefaultFrameType,PrintFrame_DefaultFrame);}
  else if(strncasecmp(frameID,"GRID",4)==0){ PRINT_LIST_FRAME(ID3Tag->GRIDFrameList,GRIDFrame,ID3v2DefaultFrameType,PrintFrame_DefaultFrame);}


  else if(strncasecmp(frameID,"IPLS",4)==0){ if(ID3Tag->IPLS != NULL) PrintFrame_IPLS(*ID3Tag->IPLS,version);}
  else if(strncasecmp(frameID,"SYTC",4)==0){ if(ID3Tag->SYTC != NULL) PrintFrame_DefaultFrame(*ID3Tag->SYTC,version);}
  else if(strncasecmp(frameID,"USER",4)==0){ if(ID3Tag->USER != NULL) PrintFrame_USER(*ID3Tag->USER,version);}
  else if(strncasecmp(frameID,"OWNE",4)==0){ if(ID3Tag->OWNE != NULL) PrintFrame_DefaultFrame(*ID3Tag->OWNE,version);}
  // else if(strncasecmp(frameID,"IPLS",4)==0){ if(ID3Tag->IPLS != NULL) PrintFrame_IPLS(*ID3Tag->IPLS);}
  // else if(strncasecmp(frameID,"SYTC",4)==0){ if(ID3Tag->SYTC != NULL) PrintFrame_SYTC(*ID3Tag->SYTC);}
  // else if(strncasecmp(frameID,"USER",4)==0){ if(ID3Tag->USER != NULL) PrintFrame_USER(*ID3Tag->USER);}
  // else if(strncasecmp(frameID,"OWNE",4)==0){ if(ID3Tag->OWNE != NULL) PrintFrame_OWNE(*ID3Tag->OWNE);}
  
  else if(strncasecmp(frameID,"COMR",4)==0){ if(ID3Tag->COMR != NULL) PrintFrame_DefaultFrame(*ID3Tag->COMR,version);}
  else if(strncasecmp(frameID,"POSS",4)==0){ if(ID3Tag->POSS != NULL) PrintFrame_DefaultFrame(*ID3Tag->POSS,version);}
  else if(strncasecmp(frameID,"RVRB",4)==0){ if(ID3Tag->RVRB != NULL) PrintFrame_RVRB(*ID3Tag->RVRB,version);}
  else if(strncasecmp(frameID,"EQUA",4)==0){ if(ID3Tag->EQUA != NULL) PrintFrame_DefaultFrame(*ID3Tag->EQUA,version);}
  else if(strncasecmp(frameID,"MLLT",4)==0){ if(ID3Tag->MLLT != NULL) PrintFrame_DefaultFrame(*ID3Tag->MLLT,version);}
  else if(strncasecmp(frameID,"ETCO",4)==0){ if(ID3Tag->ETCO != NULL) PrintFrame_DefaultFrame(*ID3Tag->ETCO,version);}
  else if(strncasecmp(frameID,"RVAD",4)==0){ if(ID3Tag->RVAD != NULL) PrintFrame_DefaultFrame(*ID3Tag->RVAD,version);}
  else{
    printf("\nNot supported tag\n");
  }
}

void PrintFrame_listFrames(ID3TagType *ID3Tag){
  int Count;
  char buffer[50];
  ENUMERATE_LIST_SINGLE_FRAME(TXTFrameList,TXTFramePtr,ID3v2TXTFrameType);
  ENUMERATE_LIST_FRAME(TXXXFrameList,"TXXX");
  ENUMERATE_LIST_FRAME(APICFrameList,"APIC");
  ENUMERATE_LIST_SINGLE_FRAME(WWWFrameList,WWWFramePtr,ID3v2WWWFrameType);
  ENUMERATE_LIST_FRAME(WXXXFrameList,"WXXX");
  ENUMERATE_LIST_FRAME(COMMFrameList,"COMM");
  ENUMERATE_LIST_FRAME(POPMFrameList,"POPM");
  if(ID3Tag->PCNT != NULL) printf("FramgeID: PCNT\n");
  ENUMERATE_LIST_FRAME(PRIVFrameList,"PRIV");
  if(ID3Tag->MCDI != NULL) printf("FramgeID: MCDI\n");

  ENUMERATE_LIST_FRAME(UFIDFrameList,"UFID");
  ENUMERATE_LIST_FRAME(USLTFrameList,"USLT");
  ENUMERATE_LIST_FRAME(SYLTFrameList,"SYLT");
  ENUMERATE_LIST_FRAME(GEOBFrameList,"GEOB");
  ENUMERATE_LIST_FRAME(LINKFrameList,"LINK");
  ENUMERATE_LIST_FRAME(AENCFrameList,"AENC");
  ENUMERATE_LIST_FRAME(ENCRFrameList,"ENCR");
  ENUMERATE_LIST_FRAME(GRIDFrameList,"GRID");

  if(ID3Tag->IPLS != NULL) printf("FramgeID: IPLS\n");
  if(ID3Tag->SYTC != NULL) printf("FramgeID: SYTC\n");
  if(ID3Tag->USER != NULL) printf("FramgeID: USER\n");
  if(ID3Tag->OWNE != NULL) printf("FramgeID: OWNE\n");

  if(ID3Tag->COMR != NULL) printf("FramgeID: COMR\n");
  if(ID3Tag->POSS != NULL) printf("FramgeID: POSS\n");
  if(ID3Tag->RVRB != NULL) printf("FramgeID: RVRB\n");
  if(ID3Tag->EQUA != NULL) printf("FramgeID: EQUA\n");
  if(ID3Tag->MLLT != NULL) printf("FramgeID: MLLT\n");
  if(ID3Tag->ETCO != NULL) printf("FramgeID: ETCO\n");
  if(ID3Tag->RVAD != NULL) printf("FramgeID: RVAD\n");
  
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
  printf("Content: ");
  PRINT_TEXTSTR(frame,content);

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
  printf("FrameData: ");
  for (size_t i = 0; i < 10 && i < DefaultFrame.size; i++) {
    printf("0x%02X ", DefaultFrame.frameData[i]);
  }
  if (DefaultFrame.size > 10 ) printf("...");
  printf("\n");
}

void PrintFrame_IPLS(ID3v2IPLSFrameType IPLS, int version){
  printf("\n----FRAME----\n");
  printf("Frame ID: %s\n",IPLS.header.frameId);
  printf("Flags: %u %u\n",IPLS.header.flags[0],IPLS.header.flags[1]);
  size_t size = FramesV2_getFrameSize(version,IPLS.header);
  printf("Size: %ld\n",size);
  printf("TextEncoding: %d\n",IPLS.textEncoding);
  printf("People List: ");
  PRINT_TEXTSTR(IPLS,peopeList);
}
void PrintFrame_RVRB(ID3v2RVRBFrameType RVRB, int version){
  printf("\n----FRAME----\n");
  printf("Frame ID: %s\n",RVRB.header.frameId);
  printf("Flags: %u %u\n",RVRB.header.flags[0],RVRB.header.flags[1]);
  size_t size = FramesV2_getFrameSize(version,RVRB.header);
  printf("Size: %ld\n",size);
  printf("Reverb left: %d\n",RVRB.left);
  printf("Reverb right: %d\n",RVRB.right);
  printf("Reverb bounces, left: %d\n",RVRB.bouncesL);
  printf("Reverb bounces, right: %d\n",RVRB.bouncesR);
  printf("Reverb feedback, left to left: %d\n",RVRB.feedBackLL);
  printf("Reverb feedback, left to right: %d\n",RVRB.feedBackLLR);
  printf("Reverb feedback, right to right: %d\n",RVRB.feedBackRR);
  printf("Reverb feedback, right to left: %d\n",RVRB.feedBackRL);
  printf("Premix left to right: %d\n",RVRB.premixLR);
  printf("Premix right to left: %d\n",RVRB.premixRL);

}
void PrintFrame_USER(ID3v2USERFrameType USER, int version){
  printf("\n----FRAME----\n");
  printf("Frame ID: %s\n",USER.header.frameId);
  printf("Flags: %u %u\n",USER.header.flags[0],USER.header.flags[1]);
  size_t size = FramesV2_getFrameSize(version,USER.header);
  printf("Size: %ld\n",size);
  printf("TextEncoding: %d\n",USER.textEncoding);
  printf("Language: %s\n",USER.language);
  printf("Terms of use: ");
  PRINT_TEXTSTR(USER,actualText);
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

void PrintFrame_TXXX(ID3v2TXXXFrameType TXXX,int version){
  printf("\n----FRAME----\n");
  printf("Frame ID: %s\n",TXXX.header.frameId);
  printf("Flags: %u %u\n",TXXX.header.flags[0],TXXX.header.flags[1]);
  size_t size = FramesV2_getFrameSize(version,TXXX.header);
  printf("Size: %ld\n",size);
  printf("TextEncoding: %d\n",TXXX.textEncoding);
  printf("Description: ");
  PRINT_TEXTSTR(TXXX,description);
  printf("Value: ");
  PRINT_TEXTSTR(TXXX,value);
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

void PrintFrame_PCNT(ID3v2PCNTFrameType PCNT,int version){
  printf("\n----FRAME----\n");
  printf("Frame ID: %s\n",PCNT.header.frameId);
  printf("Flags: %u %u\n",PCNT.header.flags[0],PCNT.header.flags[1]);
  size_t size = FramesV2_getFrameSize(version,PCNT.header);
  printf("Size: %ld\n",size);
  printf("Counter: %d\n",char_array_to_uint32(PCNT.counter));
}