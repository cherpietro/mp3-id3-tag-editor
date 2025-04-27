#include "ID3v2Frames.h"
#include "SizeReader.h"
#include <string.h>

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

void FramesV2_storeMDCI(FILE *mp3FilePointer, uint32_t frameSize,ID3v2MCDIFrameType** MDCI){
  *MDCI = (ID3v2MCDIFrameType *)malloc(sizeof(ID3v2MCDIFrameType));
  uint8_t *frameContent = (uint8_t *)malloc(frameSize);
  fread(frameContent, frameSize, 1, mp3FilePointer);
  char *ptr = (char *)frameContent;
  TxtStr_storeTextString(&(*MDCI)->CDTOC,ptr, frameSize);
  free(frameContent);
}

void FramesV2_storePOPM(FILE *mp3FilePointer, uint32_t frameSize,ID3v2POPMFrameType** POPM){
  *POPM = (ID3v2POPMFrameType *)malloc(sizeof(ID3v2POPMFrameType));
  size_t index = 0;
  uint8_t *frameContent = (uint8_t *)malloc(frameSize);
  fread(frameContent, frameSize, 1, mp3FilePointer);
  char *emailPtr = (char *)frameContent;
  size_t emailSize = strlen(emailPtr) + 1;
  TxtStr_storeTextString(&(*POPM)->userEmail,emailPtr,emailSize);
  index += emailSize;
  char *ratingPtr = (char *)frameContent + index ; 
  (*POPM)->rating = ratingPtr[0];
  char *counterPtr = (char *)frameContent + index +1; 
  (*POPM)->counter[0] = counterPtr[0];
  (*POPM)->counter[1] = counterPtr[1];
  (*POPM)->counter[2] = counterPtr[2];
  (*POPM)->counter[3] = counterPtr[3];
  free(frameContent);
}

void FramesV2_freeMCDI(ID3v2MCDIFrameType *MCDIFrame){
  TxtStr_freeTextString(&MCDIFrame->CDTOC);
  free(MCDIFrame);
}

void FramesV2_freePOPM(ID3v2POPMFrameType *POPMFrame){
  TxtStr_freeTextString(&POPMFrame->userEmail);
  free(POPMFrame);
}

void FramesV2_getPRIV(FILE *mp3FilePointer, uint32_t frameSize, ID3v2PRIVFrameType *frame){
  uint8_t *frameContent = (uint8_t *)malloc(frameSize);
  fread(frameContent, frameSize, 1, mp3FilePointer);

  size_t index = 0;
  char *ownerPtr = (char *)frameContent;
  size_t descSize = strlen(ownerPtr)+1; //description ALWAYS has to have \0
  TxtStr_storeTextString(&frame->owner,ownerPtr, descSize);
  index += descSize;

  char *privateDataPtr = (char *)frameContent + index ; 
  size_t contentSize = frameSize - index ; //Check this operation
  TxtStr_storeTextString(&frame->privateData,privateDataPtr, contentSize);
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
  printf("TextEncoding: %d\n",frame.textEncoding);
  printf("MIME type: %s\n",frame.mimeType.string);
  printf("apicframe.pictureType: %u\n",frame.pictureType);
  printf("apicframe.description: %s\n",frame.description.string);
  printf("apicframe.imageSize: %ld\n",frame.imageDataSize);
}

void FramesV2_saveAPICImage(ID3v2APICFrame frame){
  FILE *imageFile = fopen("cover.jpg", "wb");
  fwrite(frame.imageData, 1, frame.imageDataSize, imageFile);
  fclose(imageFile);
  printf("File saved in ./cover.jpg\n");
}

void FramesV2_printTXTF(ID3v2TextFrameType frame){
  printf("\n----FRAME----\n");
  printf("Frame ID: %s\n",frame.header.frameId);
  printf("Flags: %u %u\n",frame.header.flags[0],frame.header.flags[1]);
  printf("TextEncoding: %d\n",frame.textEncoding);
  printf("Content: ");
  for (int i = 0; i < (int)frame.content.size; i++) {
    if(frame.content.string[i] == '\0') printf(" ");
    else putchar(frame.content.string[i]);
  }
  printf("\n");

}

void FramesV2_printCOMM(ID3v2COMMFrameType frame){
  printf("\n----FRAME----\n");
  printf("Frame ID: %s\n",frame.header.frameId);
  printf("Flags: %u %u\n",frame.header.flags[0],frame.header.flags[1]);
  printf("TextEncoding: %d\n",frame.textEncoding);
  printf("Language: %s\n",frame.language);
  printf("Descript: ");
  for (int i = 0; i < (int)frame.contentDescript.size; i++) {
    if(frame.contentDescript.string[i] == '\0') printf(" ");
    else putchar(frame.contentDescript.string[i]);
  }
  printf("\n");
  printf("Text: ");
  for (int i = 0; i < (int)frame.actualText.size; i++) {
    if(frame.actualText.string[i] == '\0') printf(" ");
    else putchar(frame.actualText.string[i]);
  }
  printf("\n");
  
}

void FramesV2_printPRIV(ID3v2PRIVFrameType frame){
  printf("\n----FRAME----\n");
  printf("Frame ID: %s\n",frame.header.frameId);
  printf("Flags: %u %u\n",frame.header.flags[0],frame.header.flags[1]);
  printf("Owner: ");
  for (int i = 0; i < (int)frame.owner.size; i++) {
    if(frame.owner.string[i] == '\0') printf(" ");
    else putchar(frame.owner.string[i]);
  }
  printf("\n"); 
}

void FramesV2_storeAPIC(uint8_t *frameContent, uint32_t frameSize,ID3v2APICFrame** apic){
  *apic = (ID3v2APICFrame *)malloc(sizeof(ID3v2APICFrame));
  
  size_t index = 0;
  (*apic)->textEncoding = frameContent[index++];

  char *mimeType = (char*) frameContent + index;
  size_t mimeTypeLen = strlen(mimeType)+1;
  TxtStr_storeTextString(&(*apic)->mimeType,mimeType,mimeTypeLen);
  index+=mimeTypeLen;
  
  (*apic)->pictureType = frameContent[index++];

  char *description = (char*) frameContent + index;
  size_t descriptionLen = strlen(description)+1;
  TxtStr_storeTextString(&(*apic)->description,description,descriptionLen);
  index += descriptionLen;;

  (*apic)->imageDataSize = frameSize - index;
  (*apic)->imageData = (uint8_t *)malloc((*apic)->imageDataSize);
  memcpy((*apic)->imageData, frameContent + index, (*apic)->imageDataSize);
}

void FramesV2_freeAPIC(ID3v2APICFrame** apicFrame){
  TxtStr_freeTextString(&(*apicFrame)->mimeType);
  TxtStr_freeTextString(&(*apicFrame)->description);
  free(&(*apicFrame)->imageData);
  free(*apicFrame);
  *apicFrame = NULL;
}

