#include "ID3v2.h"
#include "SizeReader.h"
#include <string.h>

void readV2Tag(FILE *mp3FilePointer,ID3TagType *ID3Tag){
  int paddingReached;
  uint32_t tagSize;
  initID3v2Tag(ID3Tag);

  storeHeader(mp3FilePointer,&ID3Tag->header);
  if(isID3v2Tag(ID3Tag->header)){
    switch (getTagVersion(ID3Tag->header)){
      case 4:
        printTagHeader(ID3Tag->header);
        paddingReached = 0; //necessary=?
        tagSize = getTagSize(ID3Tag->header);
        while(ftell(mp3FilePointer) < tagSize + 10 && paddingReached != 1){
          // paddingReached = storeNextFrame(mp3FilePointer,ID3Tag);          
          paddingReached = storeV2_4Frame(mp3FilePointer,ID3Tag);
        }
        printTag(*ID3Tag);
        break;

      case 3:
        printTagHeader(ID3Tag->header);
        paddingReached = 0;
        tagSize = getTagSize(ID3Tag->header);
        while(ftell(mp3FilePointer) < tagSize + 10 && paddingReached != 1){
          paddingReached = storeNextFrame(mp3FilePointer,ID3Tag);
          // paddingReached = readFramev2_3(mp3FilePointer,ID3Tag);
        }
        printTag(*ID3Tag);
        while(!isEmptyTextFrameStack(&ID3Tag->textFrameStak)){
          printTextFrame(topTextFrameStack(ID3Tag->textFrameStak));
          popTextFrameStack(&ID3Tag->textFrameStak);
        }
        freeTextFrameStack(&ID3Tag->textFrameStak);
        // printf("\nSuposed Size: %u bytes\n",tagSize);
        break;
      
      default:
        printf("Not yet supported tag version\n");
        break;
      }
  }
  else{
    printf("Not ID3v2 Tag detected\n");
  }
  
  

  freeID3v2Tag(ID3Tag);
}

int storeV2_4Frame(FILE *mp3FilePointer, ID3TagType *ID3Tag){  
  ID3v2FrameHeaderType header;
  readHeaderFrame(mp3FilePointer,&header);
  uint32_t frameSize = getFrameV2_4Size(header);
  
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
int storeNextFrame(FILE *mp3FilePointer, ID3TagType *tag){
  ID3v2FrameHeaderType header;
  readHeaderFrame(mp3FilePointer,&header);
  uint32_t frameSize = getFrameV2_3Size(header);
  if (memcmp(header.frameId, "\0\0\0\0", 4) == 0) {
    printf("PADDING REACHED\n");
    return 1;
  }
  else if(strncmp(header.frameId,"T",1)==0){
    ID3v2TextFrameType *frame;
    frame =  (ID3v2TextFrameType *) malloc(sizeof(ID3v2TextFrameType));
    frame->header = header;
    getTextFrame(mp3FilePointer,frameSize, frame,tag->header.version[0]);
    
    pushTextFrameStack(&tag->textFrameStak,*frame);
    free(frame->content);
    free(frame);
  }
  else{
    printf("FRAMEID: %s\n", header.frameId);
    printf("Size: %d\n", frameSize);
    uint8_t *buffer = (uint8_t *)malloc(frameSize);
    fread(buffer, frameSize, 1, mp3FilePointer);
    free(buffer);
  }
  return 0;
}

int readFramev2_3(FILE *mp3FilePointer,ID3TagType *ID3Tag){  
  ID3v2FrameHeaderType header;
  readHeaderFrame(mp3FilePointer,&header);
  // fread(&header, sizeof(ID3v2FrameHeaderType), 1, mp3FilePointer);
  uint32_t frameSize = getFrameV2_3Size(header);
  // printf("FRAMEID: %s\n", header.frameId);

  if (memcmp(header.frameId, "\0\0\0\0", 4) == 0) {
    printf("PADDING REACHED\n");
    return 1;
  }
  if(strncmp(header.frameId,"TALB",4)==0){
    storeTextFrameV2_3Contet(mp3FilePointer,header,frameSize,&ID3Tag->TALB);
  }
  else if(strncmp(header.frameId,"TPE2",4)==0){
    storeTextFrameV2_3Contet(mp3FilePointer,header,frameSize,&ID3Tag->TPE2);
  }
  else if(strncmp(header.frameId,"TPE1",4)==0){
    storeTextFrameV2_3Contet(mp3FilePointer,header,frameSize,&ID3Tag->TPE1);
  }
  else if(strncmp(header.frameId,"TCOM",4)==0){
    storeTextFrameV2_3Contet(mp3FilePointer,header,frameSize,&ID3Tag->TCOM);
  }
  else if(strncmp(header.frameId,"TDRC",4)==0){
    storeTextFrameV2_3Contet(mp3FilePointer,header,frameSize,&ID3Tag->TDRC);
  }
  else if(strncmp(header.frameId,"TPOS",4)==0){
    storeTextFrameV2_3Contet(mp3FilePointer,header,frameSize,&ID3Tag->TPOS);
  }
  else if(strncmp(header.frameId,"TCON",4)==0){
    storeTextFrameV2_3Contet(mp3FilePointer,header,frameSize,&ID3Tag->TCON);
  }
  else if(strncmp(header.frameId,"TPE3",4)==0){
    storeTextFrameV2_3Contet(mp3FilePointer,header,frameSize,&ID3Tag->TPE3);
  }
  else if(strncmp(header.frameId,"TIT2",4)==0){
    storeTextFrameV2_3Contet(mp3FilePointer,header,frameSize,&ID3Tag->TIT2);
  }
  else if(strncmp(header.frameId,"TRCK",4)==0){
    storeTextFrameV2_3Contet(mp3FilePointer,header,frameSize,&ID3Tag->TRCK);
  }
  else if(strncmp(header.frameId,"TSSE",4)==0){
    storeTextFrameV2_3Contet(mp3FilePointer,header,frameSize,&ID3Tag->TSSE);
  }
  //same as in id3v2.4
  else if(strncmp(header.frameId,"APIC",4)==0){
    uint8_t *buffer = (uint8_t *)malloc(frameSize);
    fread(buffer, frameSize, 1, mp3FilePointer);
    ID3Tag->APIC = getAPICFrame(buffer,frameSize);
    ID3Tag->APIC->header = header;
    free(buffer);

  }
  else{
    printf("FRAMEID: %s\n", header.frameId);
    printf("Size: %d\n", frameSize);
    uint8_t *buffer = (uint8_t *)malloc(frameSize);
    fread(buffer, frameSize, 1, mp3FilePointer);
    free(buffer);
  }
  return 0;
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

void initID3v2Tag(ID3TagType *tag){
  initTextFrameStack(&tag->textFrameStak);

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
