#include "ID3v2.h"
#include <string.h>

void readV2Tag(FILE *mp3FilePointer,ID3TagType *ID3Tag){
  initID3v2Tag(ID3Tag);
  if(readHeader(mp3FilePointer,&ID3Tag->header)){
    printf("Version: 2.%d.%d\n",ID3Tag->header.version[0],ID3Tag->header.version[1]);
    printf("Flag: %u\n",ID3Tag->header.flags);
    uint32_t tagSize = syncsafeToSize(ID3Tag->header.size);
    printf("Size: %u bytes\n",tagSize);
    int paddingReached = 0;
    if(ID3Tag->header.version[0] == 4){
      while(ftell(mp3FilePointer) < tagSize + 10 && paddingReached != 1){
        paddingReached = readFrame(mp3FilePointer,ID3Tag);
      }
      printTag(*ID3Tag);
    }
    else if(ID3Tag->header.version[0] == 3){
      while(ftell(mp3FilePointer) < tagSize + 10 && paddingReached != 1){
        paddingReached = readFramev2_3(mp3FilePointer,ID3Tag);
      }
      printf("\nSuposed Size: %u bytes\n",tagSize);
    }
    else{
      printf("Not yet supported tag version\n");
    }
  }
  freeID3v2Tag(&ID3Tag);
}


void printTextFrame(ID3v2TextFrameType frame){
  printf("\n----FRAME----\n");
  printf("Frame ID: %s\n",frame.header.frameId);
  printf("Flags: %u %u\n",frame.header.flags[0],frame.header.flags[1]);
  printf("Size: %u bytes\n",syncsafeToSize(frame.header.size));
  printf("TextEncoding: %d\n",frame.textEncoding);
  printf("Content: %s\n",frame.content);
}
void printAPICFrame(ID3v2APICFrame frame){
  printf("\n----FRAME----\n");
  printf("Frame ID: %s\n",frame.header.frameId);
  printf("Flags: %u %u\n",frame.header.flags[0],frame.header.flags[1]);
  printf("Size: %u bytes\n",syncsafeToSize(frame.header.size));
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


void printTag(ID3TagType ID3Tag){
  if(ID3Tag.TALB != NULL) printTextFrame(*ID3Tag.TALB);
  if(ID3Tag.TPE1 != NULL) printTextFrame(*ID3Tag.TPE1);
  if(ID3Tag.TPE2 != NULL) printTextFrame(*ID3Tag.TPE2);
  if(ID3Tag.TCOM != NULL) printTextFrame(*ID3Tag.TCOM);
  if(ID3Tag.TDRC != NULL) printTextFrame(*ID3Tag.TDRC);
  if(ID3Tag.TPOS != NULL) printTextFrame(*ID3Tag.TPOS);
  if(ID3Tag.TCON != NULL) printTextFrame(*ID3Tag.TCON);
  if(ID3Tag.TPE3 != NULL) printTextFrame(*ID3Tag.TPE3);
  if(ID3Tag.TIT2 != NULL) printTextFrame(*ID3Tag.TIT2);
  if(ID3Tag.TRCK != NULL) printTextFrame(*ID3Tag.TRCK);
  if(ID3Tag.TSSE != NULL) printTextFrame(*ID3Tag.TSSE);

  if(ID3Tag.APIC != NULL) printAPICFrame(*ID3Tag.APIC);
}

void initID3v2Tag(ID3TagType *tag){
  tag->APIC = NULL;
  tag->TALB = NULL;
  tag->TPE1 = NULL;
  tag->TPE2 = NULL;
  tag->TCOM = NULL;
  tag->TDRC = NULL;
  tag->TPOS = NULL;
  tag->TCON = NULL;
  tag->TPE3 = NULL;
  tag->TIT2 = NULL;
  tag->TRCK = NULL;
  tag->TSSE = NULL;
  
}

void freeID3v2Tag(ID3TagType *tag){
  if(tag->TALB != NULL){
    free(tag->TALB->content);
    free(tag->TALB);
    tag->TALB = NULL;
  }
  if(tag->TPE1 != NULL){
    free(tag->TPE1->content);
    free(tag->TPE1);
    tag->TPE1 = NULL;
  }
  if(tag->TPE2 != NULL){
    free(tag->TPE2->content);
    free(tag->TPE2);
    tag->TPE2 = NULL;
  }
  if(tag->TCOM != NULL){
    free(tag->TCOM->content);
    free(tag->TCOM);
    tag->TCOM = NULL;
  }
  if(tag->TDRC != NULL){
    free(tag->TDRC->content);
    free(tag->TDRC);
    tag->TDRC = NULL;
  }
  if(tag->TPOS != NULL){
    free(tag->TPOS->content);
    free(tag->TPOS);
    tag->TPOS = NULL;
  }
  if(tag->TCON != NULL){
    free(tag->TCON->content);
    free(tag->TCON);
    tag->TCON = NULL;
  }
  if(tag->TPE3 != NULL){
    free(tag->TPE3->content);
    free(tag->TPE3);
    tag->TPE3 = NULL;
  }
  if(tag->TIT2 != NULL){
    free(tag->TIT2->content);
    free(tag->TIT2);
    tag->TIT2 = NULL;
  }
  if(tag->TRCK != NULL){
    free(tag->TRCK->content);
    free(tag->TRCK);
    tag->TRCK = NULL;
  }
  if(tag->TSSE != NULL){
    free(tag->TSSE->content);
    free(tag->TSSE);
    tag->TSSE = NULL;
  }
  if(tag->APIC != NULL){
    freeAPICFrame(tag->APIC);
    tag->APIC = NULL;
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

uint32_t sizeFromID3v23(uint8_t *size) {
  return (size[0] << 24) | (size[1] << 16) | (size[2] << 8) | size[3];
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
  // printf("Num of chars: %ld\n",strlen(contentPtr));
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
  if (memcmp(header.frameId, "\0\0\0\0", 4) == 0) {
    printf("PADDING REACHED\n");
    return 1;
  }
  if(strncmp(header.frameId,"TALB",4)==0){
    storeTextFrameContet(mp3FilePointer,header,frameSize,&ID3Tag->TALB);
  }
  else if(strncmp(header.frameId,"TPE2",4)==0){
    storeTextFrameContet(mp3FilePointer,header,frameSize,&ID3Tag->TPE2);
  }
  else if(strncmp(header.frameId,"TPE1",4)==0){
    storeTextFrameContet(mp3FilePointer,header,frameSize,&ID3Tag->TPE1);
  }
  else if(strncmp(header.frameId,"TCOM",4)==0){
    storeTextFrameContet(mp3FilePointer,header,frameSize,&ID3Tag->TCOM);
  }
  else if(strncmp(header.frameId,"TDRC",4)==0){
    storeTextFrameContet(mp3FilePointer,header,frameSize,&ID3Tag->TDRC);
  }
  else if(strncmp(header.frameId,"TPOS",4)==0){
    storeTextFrameContet(mp3FilePointer,header,frameSize,&ID3Tag->TPOS);
  }
  else if(strncmp(header.frameId,"TCON",4)==0){
    storeTextFrameContet(mp3FilePointer,header,frameSize,&ID3Tag->TCON);
  }
  else if(strncmp(header.frameId,"TPE3",4)==0){
    storeTextFrameContet(mp3FilePointer,header,frameSize,&ID3Tag->TPE3);
  }
  else if(strncmp(header.frameId,"TIT2",4)==0){
    storeTextFrameContet(mp3FilePointer,header,frameSize,&ID3Tag->TIT2);
  }
  else if(strncmp(header.frameId,"TRCK",4)==0){
    storeTextFrameContet(mp3FilePointer,header,frameSize,&ID3Tag->TRCK);
  }
  else if(strncmp(header.frameId,"TSSE",4)==0){
    storeTextFrameContet(mp3FilePointer,header,frameSize,&ID3Tag->TSSE);
  }
  else if(strncmp(header.frameId,"APIC",4)==0){
    uint8_t *buffer = (uint8_t *)malloc(frameSize);
    fread(buffer, frameSize, 1, mp3FilePointer);
    ID3Tag->APIC = getAPICFrame(buffer,frameSize);
    ID3Tag->APIC->header = header;
    free(buffer);

  }
  else{
    printf("\nUnkownFrame: %s\n", header.frameId);
    uint8_t *buffer = (uint8_t *)malloc(frameSize);
    fread(buffer, frameSize, 1, mp3FilePointer);
    free(buffer);

  }
  // printf("FRAMEID: %s\n", header.frameId);
  return 0;
}

int readFramev2_3(FILE *mp3FilePointer, ID3TagType *ID3Tag){  
  ID3v2FrameHeaderType header;
  fread(&header, sizeof(ID3v2FrameHeaderType), 1, mp3FilePointer);
  uint32_t frameSize = sizeFromID3v23(header.size);

  if (memcmp(header.frameId, "\0\0\0\0", 4) == 0) {
    printf("PADDING REACHED\n");
    return 1;
  }
  printf("%d-FRAMEID: %s\n", ID3Tag->header.version[0], header.frameId);
  printf("%d-Size: %d\n", ID3Tag->header.version[0], frameSize);
  uint8_t *buffer = (uint8_t *)malloc(frameSize);
  fread(buffer, frameSize, 1, mp3FilePointer);
  free(buffer);
  return 0;
}
