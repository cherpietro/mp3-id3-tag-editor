#include "ID3v2.h"
#include <string.h>


void initID3v2Tag(ID3TagType *tag){
  tag->APIC = NULL;
  tag->TALB = NULL;
}

void freeID3v2Tag(ID3TagType *tag){
  if(tag->TALB != NULL){
    free(tag->TALB->content);
    free(tag->TALB);
  }
}
void freeAPICFrame(ID3v2APICFrame* apicFrame){
  free(apicFrame->mimeType);
  free(apicFrame->description);
  free(apicFrame);
}

ID3v2APICFrame* getAPICFrame(uint8_t *frameContent, uint32_t frameSize){
  ID3v2APICFrame *apic = (ID3v2APICFrame *)malloc(sizeof(ID3v2APICFrame));
  
  size_t index = 0;
  apic->textEncoding = frameContent[index++];

  char *mimeType = (char*) frameContent + index;
  size_t mimeTypeLen = strlen(mimeType);
  apic->mimeType = (char *)malloc(mimeTypeLen + 1); 
  strcpy(apic->mimeType,mimeType);
  index+=mimeTypeLen + 1; //+1 to skip \0;
  
  apic->pictureType = frameContent[index++];

  char *description = (char*) frameContent + index;
  size_t descriptionLen = strlen(description);
  apic->description = (char *)malloc(descriptionLen + 1); 
  strcpy(apic->description,description);
  index += descriptionLen + 1; //+1 to skip \0;

  apic->imageDataSize = frameSize - index;
  apic->imageData = (uint8_t *)malloc(apic->imageDataSize);
  memcpy(apic->imageData, frameContent + index, apic->imageDataSize);

  return apic;
  
}
uint32_t syncsafeToSize( uint8_t *size) {
  return (size[0] << 21) | (size[1] << 14) | (size[2] << 7) | size[3];
}

int readHeader(FILE *mp3FilePointer, ID3v2HeaderType *header){
  fseek(mp3FilePointer,0,SEEK_SET);
  if (fread(header, sizeof(ID3v2HeaderType), 1, mp3FilePointer) == 1 && strncmp(header->tag, "ID3",3) == 0) {
    return 1;
  }
  return -1;
}

void storeTextFrameContet(FILE *mp3FilePointer, ID3v2FrameHeaderType header, uint32_t frameSize,ID3v2TextFrameType **frame){
  *frame =  (ID3v2TextFrameType *) malloc(sizeof(ID3v2TextFrameType));
  (*frame)->header = header;
  uint8_t *frameContent = (uint8_t *)malloc(frameSize);
  fread(frameContent, frameSize, 1, mp3FilePointer);

  int index = 0;
  (*frame)->textEncoding = frameContent[index++];
  char *contentPtr = (char *)frameContent + index;
  size_t contentLen = strlen(contentPtr);
  if(frameSize - 1 - (contentLen+1) != 0){ // we have to substract 1 byte from textEncoding and 1 from \0 byte
    printf("Error on size frame\n");
    printf("Frame size: %d; Content Len: %ld\n",frameSize,contentLen);
  }
  (*frame)->content = (char *)malloc(contentLen+1);
  strcpy((*frame)->content,contentPtr);
  printf("Num of chars: %ld\n",strlen(contentPtr));
  free(frameContent);
}

int readFrameHeader(FILE *mp3FilePointer, ID3v2FrameHeaderType *header){
  if (fread(header, sizeof(ID3v2FrameHeaderType), 1, mp3FilePointer) == 1 ) {
    return 1;
  }
  return -1;
}

int readFrame(FILE *mp3FilePointer, ID3TagType *ID3Tag){  
  ID3v2FrameHeaderType header;
  fread(&header, sizeof(ID3v2FrameHeaderType), 1, mp3FilePointer);
  uint32_t frameSize = syncsafeToSize(header.size);

  if(strncmp(header.frameId,"TALB",4)==0){
    storeTextFrameContet(mp3FilePointer,header,frameSize,&ID3Tag->TALB);
  }
  else if(strncmp(header.frameId,"TPE2",4)==0){
    storeTextFrameContet(mp3FilePointer,header,frameSize,&ID3Tag->TPE2);
  }
  else if(strncmp(header.frameId,"TPE1",4)==0){
    storeTextFrameContet(mp3FilePointer,header,frameSize,&ID3Tag->TPE1);
  }
  else if(strncmp(header.frameId,"APIC",4)==0){
    // ID3Tag ->APIC = (ID3v2APICFrame *) malloc(sizeof(ID3v2APICFrame));
    uint8_t *buffer = (uint8_t *)malloc(frameSize);
    fread(buffer, frameSize, 1, mp3FilePointer);
    free(buffer);
    printf("APIC Frame\n");

  }
  else{
    printf("\nUnkownFrame: %s\n", header.frameId);
    uint8_t *buffer = (uint8_t *)malloc(frameSize);
    fread(buffer, frameSize, 1, mp3FilePointer);
    free(buffer);

  }
  return frameSize+10;
  // if(strncmp(id3FrameHeader->frameId,"APIC",4)==0){
  //   uint8_t *buffer = (uint8_t *)malloc(frameSize);
  //   fread(buffer, frameSize, 1, mp3FilePointer);
  //   ID3v2APICFrame *apicFrame = getAPICFrame(buffer,frameSize);
  //   // printf("apicFrame->textEncoding: %u size:%d\n",apicFrame->textEncoding,sizeof(apicFrame->textEncoding));
  //   // printf("apicFrame->mimeType: %s size:%d\n",apicFrame->mimeType,strlen(apicFrame->mimeType));
  //   // printf("apicFrame->pictureType: %u size:%d\n",apicFrame->pictureType,sizeof(apicFrame->pictureType));
  //   // printf("apicFrame->description: %s size:%d\n",apicFrame->description,strlen(apicFrame->description));
  //   FILE *imageFile = fopen("cover.jpg", "wb");
  //   fwrite(apicFrame->imageData, 1, apicFrame->imageDataSize, imageFile);
  //   fclose(imageFile);

  //   freeAPICFrame(apicFrame);
  //   free(buffer);
  // }
  // return frameSize+10;
}