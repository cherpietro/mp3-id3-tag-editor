#include "FileFrameManager.h"

void FileManager_writteTXTFramesInFile(FILE *destFilePtr, ListFramePtr *TXTFrameList){
  ID3v2TXTFrameType *TXTFrame;
  ListFramePtr_setFirstActive(TXTFrameList);
  while(TXTFrameList->active != NULL){
    TXTFrame = (ID3v2TXTFrameType *)ListFramePtr_getActiveFramePtr(*TXTFrameList);
    fwrite(&TXTFrame->header,1, sizeof(TXTFrame->header),destFilePtr);
    fwrite(&TXTFrame->textEncoding,1, 1,destFilePtr);
    fwrite(TXTFrame->content.string,1, TxtStr_getStringLen(TXTFrame->content),destFilePtr);
    ListFramePtr_setNextActive(TXTFrameList);
  }
}

void FileManager_writteCOMMFramesInFile(FILE *destFilePtr, ListFramePtr *COMMFrameList){
  ID3v2COMMFrameType *COMMFrame;
  ListFramePtr_setFirstActive(COMMFrameList);
  while(COMMFrameList->active != NULL){
    COMMFrame = (ID3v2COMMFrameType *) ListFramePtr_getActiveFramePtr(*COMMFrameList);
    fwrite(&COMMFrame->header,1, sizeof(COMMFrame->header),destFilePtr);
    fwrite(&COMMFrame->textEncoding,1, 1,destFilePtr);
    fwrite(&COMMFrame->language,1, 3,destFilePtr);
    fwrite(COMMFrame->contentDescript.string,1, TxtStr_getStringLen(COMMFrame->contentDescript),destFilePtr);
    fwrite(COMMFrame->actualText.string,1, TxtStr_getStringLen(COMMFrame->actualText),destFilePtr);
    ListFramePtr_setNextActive(COMMFrameList);
  }  
}

void FileManager_writtePRIVFramesInFile(FILE *destFilePtr, ListFramePtr *PRIVFrameList){
  ID3v2PRIVFrameType *PRIVFrame;
  ListFramePtr_setFirstActive(PRIVFrameList);
  while(PRIVFrameList->active != NULL){
    PRIVFrame = (ID3v2PRIVFrameType *) ListFramePtr_getActiveFramePtr(*PRIVFrameList);
    fwrite(&PRIVFrame->header,1, sizeof(PRIVFrame->header),destFilePtr);
    fwrite(PRIVFrame->owner.string,1, TxtStr_getStringLen(PRIVFrame->owner),destFilePtr);
    fwrite(PRIVFrame->privateData.string,1, TxtStr_getStringLen(PRIVFrame->privateData),destFilePtr);
    ListFramePtr_setNextActive(PRIVFrameList);
  }  
}

void FileManager_writteMCDIFrameInFile(FILE *destFilePtr, ID3v2MCDIFrameType MCDI){
  fwrite(&MCDI.header,1, sizeof(MCDI.header),destFilePtr);
  fwrite(MCDI.CDTOC.string,1, TxtStr_getStringLen(MCDI.CDTOC),destFilePtr);  
}

void FileManager_writtePOPMFrameInFile(FILE *destFilePtr, ID3v2POPMFrameType POPM){
  fwrite(&POPM.header,1, sizeof(POPM.header),destFilePtr);
  fwrite(POPM.userEmail.string,1, TxtStr_getStringLen(POPM.userEmail),destFilePtr);
  fwrite(&POPM.rating,1, 1,destFilePtr);
  fwrite(&POPM.counter,1, 4,destFilePtr);
}

void FileManager_writteAPICFrameInFile(FILE *destFilePtr, ID3v2APICFrameType APIC){
  fwrite(&APIC.header,1, sizeof(APIC.header),destFilePtr);
  fwrite(&APIC.textEncoding,1, 1,destFilePtr);
  fwrite(APIC.mimeType.string,1, TxtStr_getStringLen(APIC.mimeType),destFilePtr);
  fwrite(&APIC.pictureType,1, 1,destFilePtr);
  fwrite(APIC.description.string,1, TxtStr_getStringLen(APIC.description),destFilePtr);
  fwrite(APIC.imageData,1,APIC.imageDataSize ,destFilePtr);
}
void FileManager_writteAPICFramesInFile(FILE *destFilePtr, ListFramePtr *APICFrameList){
  ID3v2APICFrameType *APICFrame;
  ListFramePtr_setFirstActive(APICFrameList);
  while(APICFrameList->active != NULL){
    APICFrame = (ID3v2APICFrameType *)ListFramePtr_getActiveFramePtr(*APICFrameList);
    fwrite(&APICFrame->header,1, sizeof(APICFrame->header),destFilePtr);
    fwrite(&APICFrame->textEncoding,1, 1,destFilePtr);
    fwrite(APICFrame->mimeType.string,1, TxtStr_getStringLen(APICFrame->mimeType),destFilePtr);
    fwrite(&APICFrame->pictureType,1, 1,destFilePtr);
    fwrite(APICFrame->description.string,1, TxtStr_getStringLen(APICFrame->description),destFilePtr);
    fwrite(APICFrame->imageData,1,APICFrame->imageDataSize ,destFilePtr);
    ListFramePtr_setNextActive(APICFrameList);
  }
}
void FileManager_writtePOPMFramesInFile(FILE *destFilePtr, ListFramePtr *POPMFrameList){
  ID3v2POPMFrameType *POPMFrame;
  ListFramePtr_setFirstActive(POPMFrameList);
  while(POPMFrameList->active != NULL){
    POPMFrame = (ID3v2POPMFrameType *)ListFramePtr_getActiveFramePtr(*POPMFrameList);
    fwrite(&POPMFrame->header,1, sizeof(POPMFrame->header),destFilePtr);
    fwrite(POPMFrame->userEmail.string,1, TxtStr_getStringLen(POPMFrame->userEmail),destFilePtr);
    fwrite(&POPMFrame->rating,1, 1,destFilePtr);
    fwrite(&POPMFrame->counter,1, 4,destFilePtr);
    ListFramePtr_setNextActive(POPMFrameList);
  }
}

void FileManager_writtePadding(FILE *destFilePtr, int paddingSize){
  char zero = 0;
  for (int i = 0; i < paddingSize; i++) {
      fwrite(&zero, 1, 1, destFilePtr);
  }
}