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

void FileManager_writtePRIVFramesInFile(FILE *destFilePtr, PRIVFrameList *PRIVFrameList){
  ID3v2PRIVFrameType PRIVFrame;
  ListPRIV_setFirstActive(PRIVFrameList);
  while(PRIVFrameList->active != NULL){
    PRIVFrame = ListPRIV_getActive(*PRIVFrameList);
    fwrite(&PRIVFrame.header,1, sizeof(PRIVFrame.header),destFilePtr);
    fwrite(PRIVFrame.owner.string,1, TxtStr_getStringLen(PRIVFrame.owner),destFilePtr);
    fwrite(PRIVFrame.privateData.string,1, TxtStr_getStringLen(PRIVFrame.privateData),destFilePtr);
    ListPRIV_setNextActive(PRIVFrameList);
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

void FileManager_writtePadding(FILE *destFilePtr, int paddingSize){
  char zero = 0;
  for (int i = 0; i < paddingSize; i++) {
      fwrite(&zero, 1, 1, destFilePtr);
  }
}