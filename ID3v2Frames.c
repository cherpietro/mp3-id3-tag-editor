#include "ID3v2Frames.h"
#include "SizeReader.h"
#include <string.h>

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

void readHeaderFrame(FILE *mp3FilePointer, ID3v2FrameHeaderType *header){
  fread(header, sizeof(ID3v2FrameHeaderType), 1, mp3FilePointer);
  // if (fread(header, sizeof(ID3v2FrameHeaderType), 1, mp3FilePointer) == 1 ) {
  //   return 1;
  // }
  // return -1;
}

uint32_t getFrameV2_4Size(ID3v2FrameHeaderType header){
  return syncsafeToSize(header.size);
}

uint32_t getFrameV2_3Size(ID3v2FrameHeaderType header){
  return sizeFromID3v23(header.size);
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
  // printf("Num of chars: %ld\n",strlen(contentPtr));
  free(frameContent);
}

void getTextFrame(FILE *mp3FilePointer, uint32_t frameSize, ID3v2TextFrameType *frame){
    uint8_t *frameContent = (uint8_t *)malloc(frameSize);
    fread(frameContent, frameSize, 1, mp3FilePointer);
  
    frame->textEncoding = frameContent[0];
    char *contentPtr = (char *)frameContent + 1;
    frame->content = (char *)malloc(frameSize); //we will use the textEncoding byte to store the \0 of the string
    strncpy(frame->content,contentPtr,frameSize-1);
    if(frame->content[frameSize-1] != '\0') frame->content[frameSize-1] = '\0';
    // printf("Frame size: %d; Content Len: %ld\n",frameSize,strlen((*frame)->content));
    free(frameContent);
    // return frame;

  
}
void OLD_getTextFrame(FILE *mp3FilePointer, uint32_t frameSize, ID3v2TextFrameType *frame,int version){
  if(version == 4){
    uint8_t *frameContent = (uint8_t *)malloc(frameSize);
    fread(frameContent, frameSize, 1, mp3FilePointer);

    int index = 0;
    frame->textEncoding = frameContent[index++];
    char *contentPtr = (char *)frameContent + index;
    size_t contentLen = strlen(contentPtr);
    if(frameSize - 1 - (contentLen+1) != 0){ // we have to substract 1 byte from textEncoding and 1 from \0 byte
      printf("Error on size frame\n");
      printf("Frame size: %d; Content Len: %ld\n",frameSize,contentLen);
    }
    frame->content = (char *)malloc(contentLen+1);
    strcpy(frame->content,contentPtr);
    // printf("Num of chars: %ld\n",strlen(contentPtr));
    free(frameContent);
  }
  else if(version == 3){
    uint8_t *frameContent = (uint8_t *)malloc(frameSize);
    fread(frameContent, frameSize, 1, mp3FilePointer);
  
    frame->textEncoding = frameContent[0];
    char *contentPtr = (char *)frameContent + 1;
    frame->content = (char *)malloc(frameSize); //we will use the textEncoding byte to store the \0 of the string
    strncpy(frame->content,contentPtr,frameSize-1);
    frame->content[frameSize-1] = '\0';
    // printf("Frame size: %d; Content Len: %ld\n",frameSize,strlen((*frame)->content));
    free(frameContent);
    // return frame;
  }
  
}

void storeTextFrameV2_3Contet(FILE *mp3FilePointer, ID3v2FrameHeaderType header, uint32_t frameSize,ID3v2TextFrameType **frame){
  *frame =  (ID3v2TextFrameType *) malloc(sizeof(ID3v2TextFrameType));
  (*frame)->header = header;
  uint8_t *frameContent = (uint8_t *)malloc(frameSize);
  fread(frameContent, frameSize, 1, mp3FilePointer);
  (*frame)->textEncoding = frameContent[0];
  char *contentPtr = (char *)frameContent + 1;
  (*frame)->content = (char *)malloc(frameSize); //we will use the textEncoding byte to store the \0 of the string
  strncpy((*frame)->content,contentPtr,frameSize-1);
  (*frame)->content[frameSize-1] = '\0';
  // printf("Frame size: %d; Content Len: %ld\n",frameSize,strlen((*frame)->content));
  free(frameContent);
}

void printAPICFrame(ID3v2APICFrame frame){
  printf("\n----FRAME----\n");
  printf("Frame ID: %s\n",frame.header.frameId);
  printf("Flags: %u %u\n",frame.header.flags[0],frame.header.flags[1]);
  // printf("Size: %u bytes\n",syncsafeToSize(frame.header.size));
  printf("TextEncoding: %d\n",frame.textEncoding);
  printf("apicframe.textEncoding: %u size:%ld\n",frame.textEncoding,sizeof(frame.textEncoding));
  printf("apicframe.mimeType: %s size:%ld\n",frame.mimeType,strlen(frame.mimeType));
  printf("apicframe.pictureType: %u size:%ld\n",frame.pictureType,sizeof(frame.pictureType));
  printf("apicframe.description: %s size:%ld\n",frame.description,strlen(frame.description));
  printf("apicframe.imageSize: %ld\n",frame.imageDataSize);
  FILE *imageFile = fopen("cover.jpg", "wb");
  fwrite(frame.imageData, 1, frame.imageDataSize, imageFile);
  fclose(imageFile);
}

void printTextFrame(ID3v2TextFrameType frame){
  printf("\n----FRAME----\n");
  printf("Frame ID: %s\n",frame.header.frameId);
  printf("Flags: %u %u\n",frame.header.flags[0],frame.header.flags[1]);
  // printf("Size: %u bytes\n",syncsafeToSize(frame.header.size));
  printf("TextEncoding: %d\n",frame.textEncoding);
  printf("Content: %s\n",frame.content);
}

//Init APIC Frame?
void freeAPICFrame(ID3v2APICFrame* apicFrame){
  free(apicFrame->mimeType);
  free(apicFrame->description);
  free(apicFrame);
}
