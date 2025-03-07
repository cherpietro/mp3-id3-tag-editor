#include "ID3v2Frames.h"
#include <string.h>

void freeAPICFrame(ID3v2APICFrame* apicFrame){
  free(apicFrame->mimeType);
  free(apicFrame->description);
  free(apicFrame);
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

int readFrameHeader(FILE *mp3FilePointer, ID3v2FrameHeaderType *header){
  if (fread(header, sizeof(ID3v2FrameHeaderType), 1, mp3FilePointer) == 1 ) {
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
  // printf("Num of chars: %ld\n",strlen(contentPtr));
  free(frameContent);
}

