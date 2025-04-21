#include "ID3v2.h"
#include "SizeReader.h"
#include <string.h>

void remove_id3v2_tag(FILE *mp3FilePointer,ID3TagType *ID3Tag){
  fseek(mp3FilePointer, 0, SEEK_END);
  // printf("pipO");
  uint32_t fileSize = ftell(mp3FilePointer);
  storeHeader(mp3FilePointer,&ID3Tag->header);
  if(isID3v2Tag(ID3Tag->header)){
    if(getTagVersion(ID3Tag->header) == 3 || getTagVersion(ID3Tag->header) == 4){
      uint32_t tagSize = getTagSize(ID3Tag->header);
      fseek(mp3FilePointer, tagSize, SEEK_SET);
      uint32_t remainingSize = fileSize - tagSize;
      unsigned char *dataBuffer = (unsigned char *)malloc(remainingSize);
      if (!dataBuffer) {
          printf("error");
          return;
      }
      size_t bytesRead = fread(dataBuffer, 1, remainingSize, mp3FilePointer);
      FILE *temp = fopen("temp.mp3", "wb");
      if (!temp) {
        printf("error");
        free(dataBuffer);
        return;
      }
      fwrite(dataBuffer, 1, bytesRead, temp);
      fclose(temp);
      free(dataBuffer);

      // remove(filename);
      // rename("temp.mp3", filename);
    }
  }
  else printf("pipO");
}
void getRealSizeTag(ID3TagType *ID3Tag){
  printf("\nsize in tag: %d bytes\n",getTagSize(ID3Tag->header));
  int sizeCalculated = 10; //header
  ID3v2TextFrameType textFrame;
  ID3v2COMMFrameType COMMFrame;

  setFirstActiveTextFrameList(&ID3Tag->textFrameList);
  while(ID3Tag->textFrameList.active != NULL){
    // printf("iter\n");
    textFrame = getTextFrameListActive(ID3Tag->textFrameList);
    if(ID3Tag->header.version[0] == 4) sizeCalculated = sizeCalculated + getFrameV2_4Size(textFrame.header) + 10;//+1 because of \0 
    else sizeCalculated = sizeCalculated + getFrameV2_3Size(textFrame.header) + 10;
    setNextActiveTextFrameList(&ID3Tag->textFrameList);
  }
  setFirstActiveCOMMFrameList(&ID3Tag->COMMFrameList);
  while(ID3Tag->COMMFrameList.active != NULL){
    // printf("iter\n");
    COMMFrame = getCOMMFrameListActive(ID3Tag->COMMFrameList);
    if(ID3Tag->header.version[0] == 4) sizeCalculated = sizeCalculated + getFrameV2_4Size(COMMFrame.header) + 10;//+1 because of \0 
    else sizeCalculated = sizeCalculated + getFrameV2_3Size(COMMFrame.header) + 10;
    setNextActiveCOMMFrameList(&ID3Tag->COMMFrameList);
  }
  if(ID3Tag->APIC != NULL) {
    size_t headerAPICsize;
    if(ID3Tag->header.version[0] == 4) headerAPICsize = getFrameV2_4Size(ID3Tag->APIC->header)+10;
    else headerAPICsize = getFrameV2_3Size(ID3Tag->APIC->header)+10;
    sizeCalculated = sizeCalculated + headerAPICsize ;
    // printf("CALCULATING APIC\n");
  }
  printf("Padding size: %ld bytes\n",ID3Tag->paddingSize);
  printf("size calculated: %d bytes\n",sizeCalculated);
  printf("Tag size removing padding: %ld bytes\n",(getTagSize(ID3Tag->header)+10)-ID3Tag->paddingSize);

}
void readV2Tag(FILE *mp3FilePointer,ID3TagType *ID3Tag){
  int paddingReached;
  uint32_t tagSize;
  initID3v2Tag(ID3Tag);

  storeHeader(mp3FilePointer,&ID3Tag->header);
  if(isID3v2Tag(ID3Tag->header)){
    if(getTagVersion(ID3Tag->header) == 3 || getTagVersion(ID3Tag->header) == 4){
      printTagHeader(ID3Tag->header);
      paddingReached = 0;
      tagSize = getTagSize(ID3Tag->header);
      while(ftell(mp3FilePointer)< tagSize + 10 && paddingReached != 1){
        paddingReached = storeNextFrame(mp3FilePointer,ID3Tag);
        // printf("PIPO %ld\n",ftell(mp3FilePointer));
      }
      if (paddingReached){
        // printf("Inicio del padding en: %ld\n",ftell(mp3FilePointer)-10);
        ID3Tag->paddingSize = (getTagSize(ID3Tag->header))+10 - (ftell(mp3FilePointer))+10; //tag size doesn't include header 
        // printf("padding size (3607): %ld\n",ID3Tag->paddingSize);
      }
      // printf("PEPE %ld\n",tagSize);
      // while(!isEmptyTextFrameList(ID3Tag->textFrameList)){
      //   setFirstActiveTextFrameList(&ID3Tag->textFrameList);
      //   printTextFrame(getTextFrameListActive(ID3Tag->textFrameList));
      //   deleteActiveTextFrameList(&ID3Tag->textFrameList);
      // }
      // if(ID3Tag->APIC != NULL) printAPICFrame(*ID3Tag->APIC);
    }
    else{
      printf("Not yet supported tag version\n");
    }
  }
  else{
    printf("Not ID3v2 Tag detected\n");
  }
  
  

  // freeID3v2Tag(ID3Tag);
}

int storeNextFrame(FILE *mp3FilePointer, ID3TagType *tag){
  ID3v2FrameHeaderType header;
  // printf("before reading header: %ld\n",ftell(mp3FilePointer));
  readHeaderFrame(mp3FilePointer,&header);
  uint32_t frameSize; 
  if(tag->header.version[0] == 4) frameSize = getFrameV2_4Size(header); 
  else frameSize = getFrameV2_3Size(header);
  if (memcmp(header.frameId, "\0\0\0\0", 4) == 0) {
    printf("PADDING REACHED\n");
    return 1;
  }
  else if(strncmp(header.frameId,"T",1)==0){
    ID3v2TextFrameType *frame;
    frame =  (ID3v2TextFrameType *) malloc(sizeof(ID3v2TextFrameType));
    frame->header = header;
    getTextFrame(mp3FilePointer,frameSize, frame);
    
    insertLastTextFrameList(&tag->textFrameList,*frame);
    free(frame->content);
    free(frame);
  }
  else if(strncmp(header.frameId,"COMM",4)==0){
    ID3v2COMMFrameType *frame;
    frame =  (ID3v2COMMFrameType *) malloc(sizeof(ID3v2COMMFrameType));
    frame->header = header;
    getCOMMFrame(mp3FilePointer,frameSize, frame);
    
    insertLastCOMMFrameList(&tag->COMMFrameList,*frame);
    free(frame->actualText);
    free(frame->contentDescript);
    free(frame);
  }
  else if(strncmp(header.frameId,"APIC",4)==0){
    uint8_t *buffer = (uint8_t *)malloc(frameSize);
    fread(buffer, frameSize, 1, mp3FilePointer);
    tag->APIC = getAPICFrame(buffer,frameSize);
    tag->APIC->header = header;
    free(buffer);

  }
  else{
    printf("NOT SUPPORTED TAG %s: %ld\n", header.frameId,ftell(mp3FilePointer));
    // printf("FRAMEID: %s\n", header.frameId);
    // printf("Size: %d\n", frameSize);
    uint8_t *buffer = (uint8_t *)malloc(frameSize);
    fread(buffer, frameSize, 1, mp3FilePointer);
    free(buffer);
  }
  return 0;
}

void freeID3v2Tag(ID3TagType *tag){
  freeTextFrameList(&tag->textFrameList);
  if(tag->APIC != NULL){
    freeAPICFrame(tag->APIC);
    tag->APIC = NULL;
  }

}

void initID3v2Tag(ID3TagType *tag){
  initTextFrameList(&tag->textFrameList);
  tag->APIC = NULL;
}