#include "ID3v2.h"
#include "SizeReader.h"
#include <string.h>

void ID3v2_storeTagInStruct(FILE *mp3FilePointer,ID3TagType *ID3Tag){
  int paddingReached = 0;
  uint32_t tagSize;
  ID3v2_init(ID3Tag);

  HeaderV2_storeHeader(mp3FilePointer,&ID3Tag->header);
  if(HeaderV2_isID3v2Tag(ID3Tag->header)){
    if(HeaderV2_getTagVersion(ID3Tag->header) == 3 || HeaderV2_getTagVersion(ID3Tag->header) == 4){
      HeaderV2_printTagHeader(ID3Tag->header);
      tagSize = HeaderV2_getTagSize(ID3Tag->header);

      while(paddingReached != 1 && ftell(mp3FilePointer) < tagSize + 10){
        paddingReached = ID3v2_storeNextFrameInStruct(mp3FilePointer,ID3Tag);
      }
      if (paddingReached){
        ID3Tag->paddingSize = (HeaderV2_getTagSize(ID3Tag->header))+10 - (ftell(mp3FilePointer))+10; //tag size doesn't include header 
      }

    }
    else{
      printf("Not yet supported tag version\n");
    }
  }
  else{
    printf("Not ID3v2 Tag detected\n");
  }
}

void ID3v2_writteTagIntoFile(FILE *mp3FilePointer, ID3TagType *ID3Tag){
  //check the file has no tag
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
  
  //COMMFrames
  ID3v2COMMFrameType COMMFrame;
  ListCOMM_setFirstActive(&ID3Tag->COMMFrameList);
  while(ID3Tag->COMMFrameList.active != NULL){
    COMMFrame = ListCOMM_getActive(ID3Tag->COMMFrameList);
    fwrite(&COMMFrame.header,1, sizeof(COMMFrame.header),temp);
    fwrite(&COMMFrame.textEncoding,1, 1,temp);
    fwrite(&COMMFrame.language,1, 3,temp);
    fwrite(COMMFrame.contentDescript,1, (size_t)(strlen(COMMFrame.contentDescript)+1),temp);
    fwrite(COMMFrame.actualText,1, (size_t)(strlen(COMMFrame.actualText)),temp);
    ListCOMM_setNextActive(&ID3Tag->COMMFrameList);
  }
  
  ID3v2TextFrameType textFrame;
  
  // content
  fwrite(dataBuffer,1,fileSize,temp);
  fclose(temp);

}

void ID3v2_removeTagFromFile(FILE *mp3FilePointer){
  fseek(mp3FilePointer, 0, SEEK_SET);
  uint32_t fileSize = ftell(mp3FilePointer);
  ID3v2HeaderType header;
  HeaderV2_storeHeader(mp3FilePointer,&header);
  if(HeaderV2_isID3v2Tag(header)){
    if(HeaderV2_getTagVersion(header) == 3 || HeaderV2_getTagVersion(header) == 4){
      uint32_t tagSize = HeaderV2_getTagSize(header);
      fseek(mp3FilePointer, tagSize, SEEK_SET);
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
    }
  }
  else printf("error, there is no ID3v2 Tag\n");
}

void ID3v2_free(ID3TagType *tag){
  ListTXTF_freeList(&tag->textFrameList);
  ListCOMM_freeList(&tag->COMMFrameList);
  if(tag->APIC != NULL){
    FramesV2_freeAPIC(tag->APIC);
    tag->APIC = NULL;
  }

}

void ID3v2_init(ID3TagType *tag){
  ListTXTF_init(&tag->textFrameList);
  ListCOMM_init(&tag->COMMFrameList);
  tag->APIC = NULL;
}

void ID3v2_getTagSizeOfTheStruct(ID3TagType *ID3Tag){
  printf("\nsize in tag: %d bytes\n",HeaderV2_getTagSize(ID3Tag->header));
  size_t tagSizeOfStruct = 10; //header
  ID3v2TextFrameType textFrame;
  ID3v2COMMFrameType COMMFrame;

  ListTXTF_setFirstActive(&ID3Tag->textFrameList);
  while(ID3Tag->textFrameList.active != NULL){
    textFrame = ListTXTF_getActive(ID3Tag->textFrameList);
    tagSizeOfStruct += (ID3Tag->header.version[0] == 4 
      ? FramesV2_getSize_V2_4(textFrame.header) 
      : FramesV2_getSize_V2_3(textFrame.header)) + 10;
    ListTXTF_setNextActive(&ID3Tag->textFrameList);
  }

  ListCOMM_setFirstActive(&ID3Tag->COMMFrameList);
  while(ID3Tag->COMMFrameList.active != NULL){
    COMMFrame = ListCOMM_getActive(ID3Tag->COMMFrameList);
    tagSizeOfStruct += (ID3Tag->header.version[0] == 4 
              ? FramesV2_getSize_V2_4(COMMFrame.header) 
              : FramesV2_getSize_V2_3(COMMFrame.header)) + 10;
    printf("size COMM: %d\n",FramesV2_getSize_V2_4(COMMFrame.header));
    ListCOMM_setNextActive(&ID3Tag->COMMFrameList);
  }
  if(ID3Tag->APIC != NULL) {
    size_t headerAPICsize;
    headerAPICsize = (ID3Tag->header.version[0] == 4 
      ? FramesV2_getSize_V2_4(ID3Tag->APIC->header) 
      : FramesV2_getSize_V2_3(ID3Tag->APIC->header)) + 10;
    tagSizeOfStruct = tagSizeOfStruct + headerAPICsize ;
  }

  if((HeaderV2_getTagSize(ID3Tag->header) +10 - ID3Tag->paddingSize )== tagSizeOfStruct ){
    printf("size is okay\n");
  }
  else{
    printf("size is NOT okay %ld\n",(tagSizeOfStruct+10 - (int) ID3Tag->paddingSize ));
  }
  // printf("\nPadding size: %ld bytes\n",ID3Tag->paddingSize);
  // printf("size calculated: %ld bytes\n",tagSizeOfStruct);
  // printf("Tag size removing padding: %ld bytes\n",(HeaderV2_getTagSize(ID3Tag->header)+10)-ID3Tag->paddingSize);

}

int ID3v2_storeNextFrameInStruct(FILE *mp3FilePointer, ID3TagType *tag){
  ID3v2FrameHeaderType header;
  FramesV2_storeHeader(mp3FilePointer,&header);
  uint32_t frameSize; 
  if(tag->header.version[0] == 4) frameSize = FramesV2_getSize_V2_4(header); 
  else frameSize = FramesV2_getSize_V2_3(header);
  // printf("FRAMEID: %s\n", header.frameId);

  if (memcmp(header.frameId, "\0\0\0\0", 4) == 0) {
    printf("PADDING REACHED\n");
    return 1;
  }
  else if(strncmp(header.frameId,"T",1)==0){
    ID3v2TextFrameType *frame;
    frame =  (ID3v2TextFrameType *) malloc(sizeof(ID3v2TextFrameType));
    frame->header = header;
    FramesV2_storeTXTF(mp3FilePointer,frameSize, frame);
    
    ListTXTF_insertLast(&tag->textFrameList,*frame);
    free(frame->content);
    free(frame);
  }
  else if(strncmp(header.frameId,"COMM",4)==0){
    ID3v2COMMFrameType *frame;
    frame =  (ID3v2COMMFrameType *) malloc(sizeof(ID3v2COMMFrameType));
    frame->header = header;
    FramesV2_getCOMM(mp3FilePointer,frameSize, frame);
    
    ListCOMM_insertLast(&tag->COMMFrameList,*frame);
    free(frame->actualText);
    free(frame->contentDescript);
    free(frame);
  }
  else if(strncmp(header.frameId,"APIC",4)==0){
    uint8_t *buffer = (uint8_t *)malloc(frameSize);
    fread(buffer, frameSize, 1, mp3FilePointer);
    FramesV2_storeAPIC(buffer,frameSize,&tag->APIC);
    // tag->APIC = FramesV2_getAPICFromBuffer(buffer,frameSize);
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