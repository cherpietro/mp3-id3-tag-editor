#include "ID3v2.h"
#include <string.h>

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

int readFrameHeader(FILE *mp3FilePointer, ID3v2DefaulFrameHeaderType *header){
  if (fread(header, sizeof(ID3v2DefaulFrameHeaderType), 1, mp3FilePointer) == 1 ) {
    return 1;
  }
  return -1;
}

int readFrame(FILE *mp3FilePointer, ID3v2DefaulFrameHeaderType *id3FrameHeader){
  printf("\n----FRAME----\n");
  fread(id3FrameHeader, sizeof(ID3v2DefaulFrameHeaderType), 1, mp3FilePointer);
  printf("Frame ID: %s\n",id3FrameHeader->frameId);
  printf("Flags: %u %u\n",id3FrameHeader->flags[0],id3FrameHeader->flags[1]);
  uint32_t frameSize = syncsafeToSize(id3FrameHeader->size);
  printf("Size: %u bytes\n",frameSize);
  if(strncmp(id3FrameHeader->frameId,"APIC",4)==0){
    uint8_t *buffer = (uint8_t *)malloc(frameSize);
    fread(buffer, frameSize, 1, mp3FilePointer);
    ID3v2APICFrame *apicFrame = getAPICFrame(buffer,frameSize);
    // printf("apicFrame->textEncoding: %u size:%d\n",apicFrame->textEncoding,sizeof(apicFrame->textEncoding));
    // printf("apicFrame->mimeType: %s size:%d\n",apicFrame->mimeType,strlen(apicFrame->mimeType));
    // printf("apicFrame->pictureType: %u size:%d\n",apicFrame->pictureType,sizeof(apicFrame->pictureType));
    // printf("apicFrame->description: %s size:%d\n",apicFrame->description,strlen(apicFrame->description));
    FILE *imageFile = fopen("cover.jpg", "wb");
    fwrite(apicFrame->imageData, 1, apicFrame->imageDataSize, imageFile);
    fclose(imageFile);

    freeAPICFrame(apicFrame);
    free(buffer);
  }
  else{
    char *buffer = (char *)malloc(frameSize + 1);
    fread(buffer, frameSize, 1, mp3FilePointer);
    buffer[frameSize] ='\0';
    printf("Content: %s\n",buffer);
    free(buffer);

  }

  return frameSize+10;
}