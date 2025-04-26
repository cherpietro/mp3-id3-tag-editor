#include "ID3v2Frames.h"
#include "SizeReader.h"
#include <string.h>

void FramesV2_storeAPIC(uint8_t *frameContent, uint32_t frameSize,ID3v2APICFrame** apic){
  *apic = (ID3v2APICFrame *)malloc(sizeof(ID3v2APICFrame));
  
  size_t index = 0;
  (*apic)->textEncoding = frameContent[index++];

  char *mimeType = (char*) frameContent + index;
  size_t mimeTypeLen = strlen(mimeType);
  (*apic)->mimeType = (char *)malloc(mimeTypeLen + 1); 
  strcpy((*apic)->mimeType,mimeType);
  index+=mimeTypeLen + 1; //+1 to skip \0;
  
  (*apic)->pictureType = frameContent[index++];

  char *description = (char*) frameContent + index;
  size_t descriptionLen = strlen(description);
  (*apic)->description = (char *)malloc(descriptionLen + 1); 
  strcpy((*apic)->description,description);
  index += descriptionLen + 1; //+1 to skip \0;

  (*apic)->imageDataSize = frameSize - index;
  (*apic)->imageData = (uint8_t *)malloc((*apic)->imageDataSize);
  memcpy((*apic)->imageData, frameContent + index, (*apic)->imageDataSize);
}

void FramesV2_storeHeader(FILE *mp3FilePointer, ID3v2FrameHeaderType *header){
  fread(header, sizeof(ID3v2FrameHeaderType), 1, mp3FilePointer);
}

uint32_t FramesV2_getSize_V2_4(ID3v2FrameHeaderType header){
  return syncsafeToSize(header.size);
}

uint32_t FramesV2_getSize_V2_3(ID3v2FrameHeaderType header){
  return sizeFromID3v23(header.size);
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

void FramesV2_storeTXTF(FILE *mp3FilePointer, uint32_t frameSize, ID3v2TextFrameType *frame){
  uint8_t *frameContent = (uint8_t *)malloc(frameSize);
  fread(frameContent, frameSize, 1, mp3FilePointer);

  frame->textEncoding = frameContent[0];
  char *contentPtr = (char *)frameContent + 1;
  TxtStr_storeTextString(&frame->content,contentPtr, frameSize-1);
  free(frameContent);
}
void FramesV2_printAPIC(ID3v2APICFrame frame){
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

void FramesV2_printTXTF(ID3v2TextFrameType frame){
  printf("\n----FRAME----\n");
  printf("Frame ID: %s\n",frame.header.frameId);
  printf("Flags: %u %u\n",frame.header.flags[0],frame.header.flags[1]);
  // printf("Size: %u bytes\n",syncsafeToSize(frame.header.size));
  printf("TextEncoding: %d\n",frame.textEncoding);
  printf("Content: %s\n",frame.content.string);
}

//Init APIC Frame?
void FramesV2_freeAPIC(ID3v2APICFrame* apicFrame){
  free(apicFrame->mimeType);
  free(apicFrame->description);
  free(apicFrame);
}
