#include "ID3v2.h"
#include "SizeReader.h"
#include "FileFrameManager.h"
#include "PrintFrameManager.h"

#include <string.h>

void ID3v2_init(ID3TagType *ID3Tag){
  ListTXTF_init(&ID3Tag->TXTFrameList);
  ListCOMM_init(&ID3Tag->COMMFrameList);
  ListPRIV_init(&ID3Tag->PRIVFrameList);
  ID3Tag->APIC = NULL;
  ID3Tag->MCDI = NULL;
  ID3Tag->POPM = NULL;
  ID3Tag->IPLS = NULL;
  ID3Tag->SYTC = NULL;
  ID3Tag->USER = NULL;
  ID3Tag->OWNE = NULL;
  ID3Tag->PCNT = NULL;
  ID3Tag->RVRB = NULL;
  ID3Tag->EQUA = NULL;
  ID3Tag->MLLT = NULL;
  ID3Tag->ETCO = NULL;
  ID3Tag->RVAD = NULL;
}

void ID3v2_free(ID3TagType *ID3Tag){
  ListTXTF_freeList(&ID3Tag->TXTFrameList);
  ListCOMM_freeList(&ID3Tag->COMMFrameList);
  ListPRIV_freeList(&ID3Tag->PRIVFrameList);
  if(ID3Tag->APIC != NULL) FramesV2_freeAPIC(&ID3Tag->APIC); 
  if(ID3Tag->MCDI != NULL) FramesV2_freeMCDI(&ID3Tag->MCDI);
  if(ID3Tag->POPM != NULL) FramesV2_freePOPM(&ID3Tag->POPM);
  if(ID3Tag->IPLS != NULL) FramesV2_freeIPLS(&ID3Tag->IPLS);
  if(ID3Tag->SYTC != NULL) FramesV2_freeSYTC(&ID3Tag->SYTC);
  if(ID3Tag->USER != NULL) FramesV2_freeUSER(&ID3Tag->USER);
  if(ID3Tag->OWNE != NULL) FramesV2_freeOWNE(&ID3Tag->OWNE);
  if(ID3Tag->PCNT != NULL) FramesV2_freePCNT(&ID3Tag->PCNT);
  if(ID3Tag->RVRB != NULL) FramesV2_freeDefaultFrame(&ID3Tag->RVRB);
  if(ID3Tag->EQUA != NULL) FramesV2_freeDefaultFrame(&ID3Tag->EQUA);
  if(ID3Tag->MLLT != NULL) FramesV2_freeDefaultFrame(&ID3Tag->MLLT);
  if(ID3Tag->ETCO != NULL) FramesV2_freeDefaultFrame(&ID3Tag->ETCO);
  if(ID3Tag->RVAD != NULL) FramesV2_freeDefaultFrame(&ID3Tag->RVAD);
}

void ID3v2_storeTagInStruct(char *file,ID3TagType *ID3Tag){
  FILE *mp3FilePointer = fopen(file,"r");
  if (mp3FilePointer) {
    int paddingReached = 0;
    HeaderV2_storeHeader(mp3FilePointer,&ID3Tag->header);
    if(HeaderV2_isID3v2Tag(ID3Tag->header)){
        uint32_t tagSize = HeaderV2_getTagSize(ID3Tag->header);
        while(!paddingReached && ftell(mp3FilePointer) < tagSize + 10){
          paddingReached = ID3v2_storeNextFrameInStruct(mp3FilePointer,ID3Tag);
        }
        if (paddingReached) ID3Tag->paddingSize = (HeaderV2_getTagSize(ID3Tag->header))+10 - (ftell(mp3FilePointer))+10; //ID3Tag size doesn't include header 
    }
    else printf("Not ID3v2 Tag detected or not yet supported version\n");
    fclose(mp3FilePointer);
  }
  else printf("The file DOESN'T exist!\n");
}

bool ID3v2_storeNextFrameInStruct(FILE *mp3FilePointer, ID3TagType *ID3Tag){
  ID3v2FrameHeaderType header;
  FramesV2_storeHeader(mp3FilePointer,&header);
  uint32_t frameSize = FramesV2_getFrameSize(ID3Tag->header.version[0],header); 
  if (memcmp(header.frameId, "\0\0\0\0", 4) == 0) return true;
  else if(strncmp(header.frameId,"T",1)==0){
    ID3v2TXTFrameType *TXTF;
    TXTF =  (ID3v2TXTFrameType *) malloc(sizeof(ID3v2TXTFrameType));
    TXTF->header = header;

    FramesV2_getTXTF(mp3FilePointer,frameSize, TXTF);
    ListTXTF_insertLast(&ID3Tag->TXTFrameList,*TXTF);
    FramesV2_freeTXTF(&TXTF);
  }
  else if(strncmp(header.frameId,"COMM",4)==0){
    ID3v2COMMFrameType *COMM;
    COMM =  (ID3v2COMMFrameType *) malloc(sizeof(ID3v2COMMFrameType));
    COMM->header = header;

    FramesV2_getCOMM(mp3FilePointer,frameSize, COMM);
    ListCOMM_insertLast(&ID3Tag->COMMFrameList,*COMM);
    FramesV2_freeCOMM(&COMM);
  }
  else if(strncmp(header.frameId,"MCDI",4)==0){
    FramesV2_storeMDCI(mp3FilePointer,frameSize, &ID3Tag->MCDI);
    ID3Tag->MCDI->header = header;
  }
  else if(strncmp(header.frameId,"POPM",4)==0){
    FramesV2_storePOPM(mp3FilePointer,frameSize, &ID3Tag->POPM);
    ID3Tag->POPM->header = header;
  }
  else if(strncmp(header.frameId,"PRIV",4)==0){
    ID3v2PRIVFrameType *PRIV;
    PRIV =  (ID3v2PRIVFrameType *) malloc(sizeof(ID3v2PRIVFrameType));
    PRIV->header = header;

    FramesV2_getPRIV(mp3FilePointer,frameSize, PRIV);
    ListPRIV_insertLast(&ID3Tag->PRIVFrameList,*PRIV);
    FramesV2_freePRIV(&PRIV);
  }
  else if(strncmp(header.frameId,"APIC",4)==0){
    uint8_t *buffer = (uint8_t *)malloc(frameSize);
    fread(buffer, frameSize, 1, mp3FilePointer);
    FramesV2_storeAPIC(buffer,frameSize,&ID3Tag->APIC);
    ID3Tag->APIC->header = header;
    free(buffer);
  }
  else if(strncmp(header.frameId,"IPLS",4)==0){
    printf("NOT TESTED TAG %s: %ld\nSize: %d\n", header.frameId,ftell(mp3FilePointer),frameSize);
    FramesV2_storeIPLS(mp3FilePointer,frameSize,&ID3Tag->IPLS);
    ID3Tag->IPLS->header = header;
  }
  else if(strncmp(header.frameId,"SYTC",4)==0){
    printf("NOT TESTED TAG %s: %ld\nSize: %d\n", header.frameId,ftell(mp3FilePointer),frameSize);
    FramesV2_storeSYTC(mp3FilePointer,frameSize,&ID3Tag->SYTC);
    ID3Tag->SYTC->header = header;
  }
  else if(strncmp(header.frameId,"USER",4)==0){
    printf("NOT TESTED TAG %s: %ld\nSize: %d\n", header.frameId,ftell(mp3FilePointer),frameSize);
    FramesV2_storeUSER(mp3FilePointer,frameSize,&ID3Tag->USER);
    ID3Tag->USER->header = header;
  }
  else if(strncmp(header.frameId,"OWNE",4)==0){
    printf("NOT TESTED TAG %s: %ld\nSize: %d\n", header.frameId,ftell(mp3FilePointer),frameSize);
    FramesV2_storeOWNE(mp3FilePointer,frameSize,&ID3Tag->OWNE);
    ID3Tag->OWNE->header = header;
  }
  else if(strncmp(header.frameId,"PCNT",4)==0){
    printf("NOT TESTED TAG %s: %ld\nSize: %d\n", header.frameId,ftell(mp3FilePointer),frameSize);
    FramesV2_storePCNT(mp3FilePointer,frameSize,&ID3Tag->PCNT);
    ID3Tag->PCNT->header = header;
  }
  else if(strncmp(header.frameId,"RVRB",4)==0){
    printf("NOT TESTED TAG %s: %ld\nSize: %d\n", header.frameId,ftell(mp3FilePointer),frameSize);
    FramesV2_storeDefaultFrame(mp3FilePointer,frameSize,&ID3Tag->RVRB);
    ID3Tag->RVRB->header = header;
  }
  else if(strncmp(header.frameId,"EQUA",4)==0){
    printf("NOT TESTED TAG %s: %ld\nSize: %d\n", header.frameId,ftell(mp3FilePointer),frameSize);
    FramesV2_storeDefaultFrame(mp3FilePointer,frameSize,&ID3Tag->EQUA);
    ID3Tag->EQUA->header = header;
  }
  else if(strncmp(header.frameId,"MLLT",4)==0){
    printf("NOT TESTED TAG %s: %ld\nSize: %d\n", header.frameId,ftell(mp3FilePointer),frameSize);
    FramesV2_storeDefaultFrame(mp3FilePointer,frameSize,&ID3Tag->MLLT);
    ID3Tag->MLLT->header = header;
  }
  else if(strncmp(header.frameId,"ETCO",4)==0){
    printf("NOT TESTED TAG %s: %ld\nSize: %d\n", header.frameId,ftell(mp3FilePointer),frameSize);
    FramesV2_storeDefaultFrame(mp3FilePointer,frameSize,&ID3Tag->ETCO);
    ID3Tag->ETCO->header = header;
  }
  else if(strncmp(header.frameId,"RVAD",4)==0){
    printf("NOT TESTED TAG %s: %ld\nSize: %d\n", header.frameId,ftell(mp3FilePointer),frameSize);
    FramesV2_storeDefaultFrame(mp3FilePointer,frameSize,&ID3Tag->RVAD);
    ID3Tag->RVAD->header = header;
  }
  else{
    printf("NOT SUPPORTED TAG %s: %ld\nSize: %d\n", header.frameId,ftell(mp3FilePointer),frameSize);
    uint8_t *buffer = (uint8_t *)malloc(frameSize);
    fread(buffer, frameSize, 1, mp3FilePointer);
    free(buffer);
  }
  return 0;
}

void ID3v2_removeTagFromFile(char*file){
  FILE *mp3FilePointer = fopen(file,"r");
  if (mp3FilePointer) {
    fseek(mp3FilePointer, 0, SEEK_SET);
    uint32_t fileSize = ftell(mp3FilePointer);
    ID3v2HeaderType header;
    HeaderV2_storeHeader(mp3FilePointer,&header);
    if(HeaderV2_isID3v2Tag(header)){
        uint32_t tagSize = HeaderV2_getTagSize(header);
        fseek(mp3FilePointer, tagSize+10, SEEK_SET);
        uint32_t remainingSize = fileSize - tagSize;
        unsigned char *dataBuffer = (unsigned char *)malloc(remainingSize);
        if (!dataBuffer) {
            printf("error\n");
            return;
        }
        size_t bytesRead = fread(dataBuffer, 1, remainingSize, mp3FilePointer);
        FILE *temp = fopen("tagRemoved.mp3", "wb");
        if (!temp) {
          printf("error\n");
          free(dataBuffer);
          return;
        }
        fwrite(dataBuffer, 1, bytesRead, temp);
        fclose(temp);
        free(dataBuffer);

        remove(file);
        rename("tagRemoved.mp3",file);
      }
    else printf("Not ID3v2 Tag detected or not yet supported version\n");
    fclose(mp3FilePointer);
  }
  else printf("The file DOESN'T exist!\n");
}

void ID3v2_writteTagIntoFile(char *file, ID3TagType *ID3Tag){
  ID3v2_removeTagFromFile(file);
  FILE *mp3FilePointer = fopen(file,"r");
  if(mp3FilePointer){
    fseek(mp3FilePointer,0,SEEK_END);
    uint32_t fileSize = ftell(mp3FilePointer);
    fseek(mp3FilePointer,0,SEEK_SET);
    unsigned char *dataBuffer = (unsigned char *)malloc(fileSize);
    if(!dataBuffer){
      printf("error");
      fclose(mp3FilePointer);
      return;
    }
    fread(dataBuffer,1,fileSize,mp3FilePointer);
    FILE *temp = fopen("temp.mp3","w");
    if(!temp){
      printf("error");
      fclose(temp);
      return;
    }
    // header
    fwrite(&ID3Tag->header,1,sizeof(ID3Tag->header),temp);

    FileManager_writteCOMMFramesInFile(temp,&ID3Tag->COMMFrameList);
    FileManager_writteTXTFramesInFile(temp,&ID3Tag->TXTFrameList);
    FileManager_writtePRIVFramesInFile(temp,&ID3Tag->PRIVFrameList);
    if(ID3Tag->MCDI != NULL) FileManager_writteMCDIFrameInFile(temp,*ID3Tag->MCDI);
    if(ID3Tag->APIC != NULL) FileManager_writteAPICFrameInFile(temp,*ID3Tag->APIC);
    if(ID3Tag->POPM != NULL) FileManager_writtePOPMFrameInFile(temp,*ID3Tag->POPM);
    FileManager_writtePadding(temp,ID3Tag->paddingSize);
    
    // content
    fwrite(dataBuffer,1,fileSize,temp);
    fclose(temp);

    remove(file);
    rename("temp.mp3",file);
  }
}

//IMPLEMENT REMAINING TAGS
void ID3v2_getTagSizeOfTheStruct(ID3TagType *ID3Tag){
  printf("\nsize in ID3Tag: %d bytes\n",HeaderV2_getTagSize(ID3Tag->header));
  size_t tagSizeOfStruct = 10; //header
  ID3v2TXTFrameType textFrame;
  ID3v2COMMFrameType COMMFrame;

  ListTXTF_setFirstActive(&ID3Tag->TXTFrameList);
  while(ID3Tag->TXTFrameList.active != NULL){
    textFrame = ListTXTF_getActive(ID3Tag->TXTFrameList);
    tagSizeOfStruct += FramesV2_getFrameSize(ID3Tag->header.version[0],textFrame.header) + 10;
    ListTXTF_setNextActive(&ID3Tag->TXTFrameList);
  }

  ListCOMM_setFirstActive(&ID3Tag->COMMFrameList);
  while(ID3Tag->COMMFrameList.active != NULL){
    COMMFrame = ListCOMM_getActive(ID3Tag->COMMFrameList);
    tagSizeOfStruct += FramesV2_getFrameSize(ID3Tag->header.version[0],COMMFrame.header) + 10;
    ListCOMM_setNextActive(&ID3Tag->COMMFrameList);
  }
  if(ID3Tag->APIC != NULL) {
    size_t headerAPICsize;
    headerAPICsize = FramesV2_getFrameSize(ID3Tag->header.version[0],ID3Tag->APIC->header) + 10;
    tagSizeOfStruct = tagSizeOfStruct + headerAPICsize ;
  }

  if((HeaderV2_getTagSize(ID3Tag->header) +10 - ID3Tag->paddingSize )== tagSizeOfStruct ){
    printf("size is okay\n");
  }
  else{
    printf("size is NOT okay %ld\n",(tagSizeOfStruct+10 - (int) ID3Tag->paddingSize ));
  }
}

void ID3v2_saveAPICImage(ID3TagType *ID3Tag){
  FramesV2_saveAPICImage(*ID3Tag->APIC);
}

void printTag(ID3TagType *ID3Tag){
  HeaderV2_printTagHeader(ID3Tag->header);
  PrintFrame_PrintTXTFrames(&ID3Tag->TXTFrameList);  
  PrintFrame_PrintCOMMFrames(&ID3Tag->COMMFrameList);
  PrintFrame_PrintPRIVFrames(&ID3Tag->PRIVFrameList);
  if(ID3Tag->APIC != NULL) FramesV2_printAPIC(*ID3Tag->APIC);
  if(ID3Tag->MCDI != NULL) FramesV2_printMDCI(*ID3Tag->MCDI);
  if(ID3Tag->POPM != NULL) FramesV2_printPOPM(*ID3Tag->POPM);
}