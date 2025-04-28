#include "ID3v2Frames.h"
#include "SizeReader.h"
#include <string.h>

void FramesV2_storeHeader(FILE *mp3FilePointer, ID3v2FrameHeaderType *header){
  fread(header, sizeof(ID3v2FrameHeaderType), 1, mp3FilePointer);
}
uint32_t FramesV2_getFrameSize(int version, ID3v2FrameHeaderType header){
  if(version == 4)  return syncsafeToSize(header.size);
  else return sizeFromID3v23(header.size);
}

void FramesV2_storeAPIC(uint8_t *frameContent, uint32_t frameSize,ID3v2APICFrameType** apic){
  *apic = (ID3v2APICFrameType *)malloc(sizeof(ID3v2APICFrameType));
  
  size_t index = 0;
  (*apic)->textEncoding = frameContent[index++];

  char *mimeType = (char*) frameContent + index;
  size_t mimeTypeLen = strlen(mimeType)+1;
  TxtStr_storeTextString(&(*apic)->mimeType,mimeType,mimeTypeLen);
  index+=mimeTypeLen;
  
  (*apic)->pictureType = frameContent[index++];

  char *description = (char*) frameContent + index;
  size_t descriptionLen = strlen(description)+1;
  TxtStr_storeTextString(&(*apic)->description,description,descriptionLen);
  index += descriptionLen;;

  (*apic)->imageDataSize = frameSize - index;
  (*apic)->imageData = (uint8_t *)malloc((*apic)->imageDataSize);
  memcpy((*apic)->imageData, frameContent + index, (*apic)->imageDataSize);
}
void FramesV2_saveAPICImage(ID3v2APICFrameType frame){
  FILE *imageFile = fopen("cover.jpg", "wb");
  fwrite(frame.imageData, 1, frame.imageDataSize, imageFile);
  fclose(imageFile);
  printf("File saved in ./cover.jpg\n");
}
void FramesV2_printAPIC(ID3v2APICFrameType frame){
  printf("\n----FRAME----\n");
  printf("Frame ID: %s\n",frame.header.frameId);
  printf("Flags: %u %u\n",frame.header.flags[0],frame.header.flags[1]);
  printf("TextEncoding: %d\n",frame.textEncoding);
  printf("MIME type: %s\n",frame.mimeType.string);
  printf("apicframe.pictureType: %u\n",frame.pictureType);
  printf("apicframe.description: %s\n",frame.description.string);
  printf("apicframe.imageSize: %ld\n",frame.imageDataSize);
}
void FramesV2_freeAPIC(ID3v2APICFrameType** APIC){
  TxtStr_freeTextString(&(*APIC)->mimeType);
  TxtStr_freeTextString(&(*APIC)->description);
  free((*APIC)->imageData);
  free(*APIC);
  *APIC = NULL;
}

void FramesV2_getTXTF(FILE *mp3FilePointer, uint32_t frameSize, ID3v2TXTFrameType *frame){
  uint8_t *frameContent = (uint8_t *)malloc(frameSize);
  fread(frameContent, frameSize, 1, mp3FilePointer);

  frame->textEncoding = frameContent[0];
  char *contentPtr = (char *)frameContent + 1;
  TxtStr_storeTextString(&frame->content,contentPtr, frameSize-1);
  free(frameContent);
}
void FramesV2_printTXTF(ID3v2TXTFrameType frame){
  printf("\n----FRAME----\n");
  printf("Frame ID: %s\n",frame.header.frameId);
  printf("Flags: %u %u\n",frame.header.flags[0],frame.header.flags[1]);
  printf("TextEncoding: %d\n",frame.textEncoding);
  printf("Content: ");
  for (int i = 0; i < (int)frame.content.size; i++) {
    if(frame.content.string[i] == '\0') printf(" ");
    else putchar(frame.content.string[i]);
  }
  printf("\n");

}
void FramesV2_freeTXTF(ID3v2TXTFrameType** TXTF){
  TxtStr_freeTextString(&(*TXTF)->content);
  free(*TXTF);
  *TXTF = NULL;
}

void FramesV2_getCOMM(FILE *mp3FilePointer, uint32_t frameSize, ID3v2COMMFrameType *frame){
  uint8_t *frameContent = (uint8_t *)malloc(frameSize);
  fread(frameContent, frameSize, 1, mp3FilePointer);

  size_t index = 0;
  frame->textEncoding = frameContent[index++];
  frame->language[0] = frameContent[index];
  frame->language[1] = frameContent[index+1];
  frame->language[2] = frameContent[index+2];
  index+=3;

  char *descPtr = (char *)frameContent + index;
  size_t descSize = strlen(descPtr)+1; //description ALWAYS has to have \0
  TxtStr_storeTextString(&frame->contentDescript,descPtr, descSize);
  index += descSize;

  char *contentPtr = (char *)frameContent + index ; 
  size_t contentSize = frameSize - index ; //Check this operation
  TxtStr_storeTextString(&frame->actualText,contentPtr, contentSize);
  free(frameContent);
}
void FramesV2_printCOMM(ID3v2COMMFrameType frame){
  printf("\n----FRAME----\n");
  printf("Frame ID: %s\n",frame.header.frameId);
  printf("Flags: %u %u\n",frame.header.flags[0],frame.header.flags[1]);
  printf("TextEncoding: %d\n",frame.textEncoding);
  printf("Language: %s\n",frame.language);
  printf("Descript: ");
  for (int i = 0; i < (int)frame.contentDescript.size; i++) {
    if(frame.contentDescript.string[i] == '\0') printf(" ");
    else putchar(frame.contentDescript.string[i]);
  }
  printf("\n");
  printf("Text: ");
  for (int i = 0; i < (int)frame.actualText.size; i++) {
    if(frame.actualText.string[i] == '\0') printf(" ");
    else putchar(frame.actualText.string[i]);
  }
  printf("\n");
  
}
void FramesV2_freeCOMM(ID3v2COMMFrameType** COMM){
  TxtStr_freeTextString(&(*COMM)->actualText);
  TxtStr_freeTextString(&(*COMM)->contentDescript);
  free(*COMM);
  *COMM = NULL;
}

void FramesV2_printPRIV(ID3v2PRIVFrameType frame){
  printf("\n----FRAME----\n");
  printf("Frame ID: %s\n",frame.header.frameId);
  printf("Flags: %u %u\n",frame.header.flags[0],frame.header.flags[1]);
  printf("Owner: ");
  for (int i = 0; i < (int)frame.owner.size; i++) {
    if(frame.owner.string[i] == '\0') printf(" ");
    else putchar(frame.owner.string[i]);
  }
  printf("\n"); 
}
void FramesV2_getPRIV(FILE *mp3FilePointer, uint32_t frameSize, ID3v2PRIVFrameType *frame){
  uint8_t *frameContent = (uint8_t *)malloc(frameSize);
  fread(frameContent, frameSize, 1, mp3FilePointer);

  size_t index = 0;
  char *ownerPtr = (char *)frameContent;
  size_t descSize = strlen(ownerPtr)+1; //description ALWAYS has to have \0
  TxtStr_storeTextString(&frame->owner,ownerPtr, descSize);
  index += descSize;

  char *privateDataPtr = (char *)frameContent + index ; 
  size_t contentSize = frameSize - index ; //Check this operation
  TxtStr_storeTextString(&frame->privateData,privateDataPtr, contentSize);
  free(frameContent);
}
void FramesV2_freePRIV(ID3v2PRIVFrameType** PRIV){
  TxtStr_freeTextString(&(*PRIV)->owner);
  TxtStr_freeTextString(&(*PRIV)->privateData);
  free(*PRIV);
  *PRIV = NULL;
}

void FramesV2_storeMDCI(FILE *mp3FilePointer, uint32_t frameSize,ID3v2MCDIFrameType** MDCI){
  *MDCI = (ID3v2MCDIFrameType *)malloc(sizeof(ID3v2MCDIFrameType));
  uint8_t *frameContent = (uint8_t *)malloc(frameSize);
  fread(frameContent, frameSize, 1, mp3FilePointer);
  char *ptr = (char *)frameContent;
  TxtStr_storeTextString(&(*MDCI)->CDTOC,ptr, frameSize);
  free(frameContent);
}
void FramesV2_printMDCI(ID3v2MCDIFrameType MDCI){
  printf("\n----FRAME----\n");
  printf("Frame ID: %s\n",MDCI.header.frameId);
  printf("Flags: %u %u\n",MDCI.header.flags[0],MDCI.header.flags[1]);
}
void FramesV2_freeMCDI(ID3v2MCDIFrameType **MCDI){
  TxtStr_freeTextString(&(*MCDI)->CDTOC);
  free(*MCDI);
  *MCDI = NULL;
}

void FramesV2_storePOPM(FILE *mp3FilePointer, uint32_t frameSize,ID3v2POPMFrameType** POPM){
  *POPM = (ID3v2POPMFrameType *)malloc(sizeof(ID3v2POPMFrameType));
  size_t index = 0;
  uint8_t *frameContent = (uint8_t *)malloc(frameSize);
  fread(frameContent, frameSize, 1, mp3FilePointer);
  char *emailPtr = (char *)frameContent;
  size_t emailSize = strlen(emailPtr) + 1;
  TxtStr_storeTextString(&(*POPM)->userEmail,emailPtr,emailSize);
  index += emailSize;
  char *ratingPtr = (char *)frameContent + index ; 
  (*POPM)->rating = ratingPtr[0];
  char *counterPtr = (char *)frameContent + index +1; 
  (*POPM)->counter[0] = counterPtr[0];
  (*POPM)->counter[1] = counterPtr[1];
  (*POPM)->counter[2] = counterPtr[2];
  (*POPM)->counter[3] = counterPtr[3];
  free(frameContent);
}
void FramesV2_printPOPM(ID3v2POPMFrameType POPM){
  printf("\n----FRAME----\n");
  printf("Frame ID: %s\n",POPM.header.frameId);
  printf("Flags: %u %u\n",POPM.header.flags[0],POPM.header.flags[1]);
  printf("User email: %s\n",POPM.userEmail.string);
  printf("Ratin: %d\n",POPM.rating);
}
void FramesV2_freePOPM(ID3v2POPMFrameType **POPM){
  TxtStr_freeTextString(&(*POPM)->userEmail);
  free(*POPM);
  *POPM = NULL;
}

void FramesV2_storeIPLS(FILE* mp3FilePointer, uint32_t frameSize, ID3v2IPLSFrameType **IPLS){
  *IPLS = (ID3v2IPLSFrameType *)malloc(sizeof(ID3v2IPLSFrameType));
  uint8_t *frameContent = (uint8_t *)malloc(frameSize);
  fread(frameContent, frameSize, 1, mp3FilePointer);

  (*IPLS)->textEncoding = frameContent[0];
  char *peopleLstPtr = (char *)frameContent + 1;
  TxtStr_storeTextString(&(*IPLS)->peopeList,peopleLstPtr,frameSize -1);
  free(frameContent);
}
void FramesV2_printIPLS(ID3v2IPLSFrameType IPLS){
  printf("\n----FRAME----\n");
  printf("Frame ID: %s\n",IPLS.header.frameId);
  printf("Flags: %u %u\n",IPLS.header.flags[0],IPLS.header.flags[1]);
  printf("TextEncoding: %d\n",IPLS.textEncoding);
  printf("People List: ");
  for (int i = 0; i < (int)IPLS.peopeList.size; i++) {
    if(IPLS.peopeList.string[i] == '\0') printf("\n");
    else putchar(IPLS.peopeList.string[i]);
  }
  printf("\n");
}
void FramesV2_freeIPLS(ID3v2IPLSFrameType **IPLS){
  TxtStr_freeTextString(&(*IPLS)->peopeList);
  free(*IPLS);
  *IPLS = NULL;
}

void FramesV2_storeSYTC(FILE* mp3FilePointer, uint32_t frameSize, ID3v2SYTCFrameType **SYTC){
  *SYTC = (ID3v2SYTCFrameType *)malloc(sizeof(ID3v2SYTCFrameType));
  uint8_t *frameContent = (uint8_t *)malloc(frameSize);
  fread(frameContent, frameSize, 1, mp3FilePointer);

  (*SYTC)->timestampFormat = frameContent[0];
  uint8_t *tempoDataPtr = (uint8_t *)frameContent + 1;
  (*SYTC)->tempoData =  (uint8_t *)malloc(frameSize-1);
  memcpy((*SYTC)->tempoData,tempoDataPtr,frameSize-1);
  free(frameContent);
}
void FramesV2_printSYTC(ID3v2SYTCFrameType SYTC){
  printf("\n----FRAME----\n");
  printf("Frame ID: %s\n",SYTC.header.frameId);
  printf("Flags: %u %u\n",SYTC.header.flags[0],SYTC.header.flags[1]);
  printf("Timestamp format: %d\n",SYTC.timestampFormat);
}
void FramesV2_freeSYTC(ID3v2SYTCFrameType **SYTC){
  free((*SYTC)->tempoData);
  free(*SYTC);
  *SYTC = NULL;
}

void FramesV2_storeUSER(FILE* mp3FilePointer, uint32_t frameSize, ID3v2USERFrameType **USER){
  *USER = (ID3v2USERFrameType *)malloc(sizeof(ID3v2USERFrameType));
  uint8_t *frameContent = (uint8_t *)malloc(frameSize);
  fread(frameContent, frameSize, 1, mp3FilePointer);

  size_t index = 0;
  (*USER)->textEncoding = frameContent[index++];
  (*USER)->language[0] = frameContent[index];
  (*USER)->language[1] = frameContent[index+1];
  (*USER)->language[2] = frameContent[index+2];
  index+=3;

  char *textPtr = (char *)frameContent + index ; 
  size_t textSize = frameSize - index ; //Check this operation
  TxtStr_storeTextString(&(*USER)->actualText,textPtr, textSize);
  free(frameContent);
}
void FramesV2_printUSER(ID3v2USERFrameType USER){
  printf("\n----FRAME----\n");
  printf("Frame ID: %s\n",USER.header.frameId);
  printf("Flags: %u %u\n",USER.header.flags[0],USER.header.flags[1]);
  printf("Text Encoding: %d\n",USER.textEncoding);
  printf("Language: %s\n",USER.language);
  printf("Text: ");
  for (int i = 0; i < (int)USER.actualText.size; i++) {
    if(USER.actualText.string[i] == '\0') printf(" ");
    else putchar(USER.actualText.string[i]);
  }
  printf("\n");
}
void FramesV2_freeUSER(ID3v2USERFrameType **USER){
  TxtStr_freeTextString(&(*USER)->actualText);
  free(*USER);
  *USER = NULL;
}

void FramesV2_storeOWNE(FILE *mp3FilePointer, uint32_t frameSize, ID3v2OWNEFrameType **OWNE){
  *OWNE = (ID3v2OWNEFrameType *)malloc(sizeof(ID3v2OWNEFrameType));
  uint8_t *frameContent = (uint8_t *)malloc(frameSize);
  fread(frameContent, frameSize, 1, mp3FilePointer);

  size_t index = 0;
  (*OWNE)->textEncoding = frameContent[index++];
  char *prizePtr = (char *)frameContent + index;
  size_t prizeSize = strlen(prizePtr)+1; //prize ALWAYS has to have \0
  TxtStr_storeTextString(&(*OWNE)->pricePayed,prizePtr, prizeSize);
  index += prizeSize;
  
  (*OWNE)->dateOfPurch[0] = frameContent[index];
  (*OWNE)->dateOfPurch[1] = frameContent[index+1];
  (*OWNE)->dateOfPurch[2] = frameContent[index+2];
  (*OWNE)->dateOfPurch[3] = frameContent[index+3];
  (*OWNE)->dateOfPurch[4] = frameContent[index+4];
  (*OWNE)->dateOfPurch[5] = frameContent[index+5];
  (*OWNE)->dateOfPurch[6] = frameContent[index+6];
  (*OWNE)->dateOfPurch[7] = frameContent[index+7];
  index += 8;
  
  char *sellerPtr = (char *)frameContent + index ; 
  size_t sellerSize = frameSize - index ; //Check this operation
  TxtStr_storeTextString(&(*OWNE)->seller,sellerPtr, sellerSize);
  free(frameContent);
}
void FramesV2_printOWNE(ID3v2OWNEFrameType OWNE){
  printf("\n----FRAME----\n");
  printf("Frame ID: %s\n",OWNE.header.frameId);
  printf("Flags: %u %u\n",OWNE.header.flags[0],OWNE.header.flags[1]);
  printf("TextEncoding: %d\n",OWNE.textEncoding);
  printf("Price payed:");
  for (int i = 0; i < (int)OWNE.pricePayed.size; i++) {
    if(OWNE.pricePayed.string[i] == '\0') printf(" ");
    else putchar(OWNE.pricePayed.string[i]);
  }
  printf("\n");
  printf("Date of pruchase: %s\n",OWNE.dateOfPurch);
  printf("Seller: ");
  for (int i = 0; i < (int)OWNE.seller.size; i++) {
    if(OWNE.seller.string[i] == '\0') printf(" ");
    else putchar(OWNE.seller.string[i]);
  }
  printf("\n");
  
}
void FramesV2_freeOWNE(ID3v2OWNEFrameType** OWNE){
  TxtStr_freeTextString(&(*OWNE)->pricePayed);
  TxtStr_freeTextString(&(*OWNE)->seller);
  free(*OWNE);
  *OWNE = NULL;
}

void FramesV2_storePCNT(FILE* mp3FilePointer, uint32_t frameSize, ID3v2PCNTFrameType **PCNT){
  *PCNT = (ID3v2PCNTFrameType *)malloc(sizeof(ID3v2PCNTFrameType));
  uint8_t *frameContent = (uint8_t *)malloc(frameSize);
  fread(frameContent, frameSize, 1, mp3FilePointer);

  char *counterPtr = (char *)frameContent;
  TxtStr_storeTextString(&(*PCNT)->counter,counterPtr,frameSize);
  free(frameContent);
}
void FramesV2_printPCNT(ID3v2PCNTFrameType PCNT){
  printf("\n----FRAME----\n");
  printf("Frame ID: %s\n",PCNT.header.frameId);
  printf("Flags: %u %u\n",PCNT.header.flags[0],PCNT.header.flags[1]);
  printf("Counter: %s\n",PCNT.counter.string);
}
void FramesV2_freePCNT(ID3v2PCNTFrameType **PCNT){
  TxtStr_freeTextString(&(*PCNT)->counter);
  free(*PCNT);
  *PCNT = NULL;
}

void FramesV2_storeDefaultFrame(FILE* mp3FilePointer, uint32_t frameSize, ID3v2DefaultFrameType **DefaultFrame){
  *DefaultFrame = (ID3v2DefaultFrameType *)malloc(sizeof(ID3v2DefaultFrameType));
  (*DefaultFrame)->frameData = (uint8_t *)malloc(frameSize);
  fread((*DefaultFrame)->frameData, frameSize, 1, mp3FilePointer);
}
void FramesV2_printDefaultFrame(ID3v2DefaultFrameType DefaultFrame){
  printf("\n----FRAME----\n");
  printf("Frame ID: %s\n",DefaultFrame.header.frameId);
  printf("Flags: %u %u\n",DefaultFrame.header.flags[0],DefaultFrame.header.flags[1]);
}
void FramesV2_freeDefaultFrame(ID3v2DefaultFrameType **DefaultFrame){
  free((*DefaultFrame)->frameData);
  free(*DefaultFrame);
  *DefaultFrame = NULL;
}
