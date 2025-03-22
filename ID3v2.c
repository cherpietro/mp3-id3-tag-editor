#include "ID3v2.h"
#include "SizeReader.h"
#include <string.h>

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
      while(ftell(mp3FilePointer) < tagSize + 10 && paddingReached != 1){
        paddingReached = storeNextFrame(mp3FilePointer,ID3Tag);
      }
      while(!isEmptyTextFrameList(ID3Tag->textFrameList)){
        setFirstActiveTextFrameList(&ID3Tag->textFrameList);
        printTextFrame(getTextFrameListActive(ID3Tag->textFrameList));
        deleteActiveTextFrameList(&ID3Tag->textFrameList);
      }
      if(ID3Tag->APIC != NULL) printAPICFrame(*ID3Tag->APIC);
    }
    else{
      printf("Not yet supported tag version\n");
    }
  }
  else{
    printf("Not ID3v2 Tag detected\n");
  }
  
  

  freeID3v2Tag(ID3Tag);
}

int storeNextFrame(FILE *mp3FilePointer, ID3TagType *tag){
  ID3v2FrameHeaderType header;
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
  else if(strncmp(header.frameId,"APIC",4)==0){
    uint8_t *buffer = (uint8_t *)malloc(frameSize);
    fread(buffer, frameSize, 1, mp3FilePointer);
    tag->APIC = getAPICFrame(buffer,frameSize);
    tag->APIC->header = header;
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

