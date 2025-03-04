#include <stdio.h>
#include <stdint.h>
#include "ID3v2.h"

void printTextFrame(ID3v2TextFrameType);

int main(){
  FILE *mp3FilePointer;
  ID3TagType ID3Tag; 
  initID3v2Tag(&ID3Tag);

  mp3FilePointer = fopen("./files/overture.mp3","r");
  if (mp3FilePointer) {
    if(readHeader(mp3FilePointer,&ID3Tag.header)){
      printf("Version: 2.%d.%d\n",ID3Tag.header.version[0],ID3Tag.header.version[1]);
      printf("Flag: %u\n",ID3Tag.header.flags);
      uint32_t tagSize = syncsafeToSize(ID3Tag.header.size);
      printf("Size: %u bytes\n",tagSize);
      
      int readedBytes,remainingBytes;
      remainingBytes = tagSize-10;
      while(remainingBytes > 0){
        readedBytes = readFrame(mp3FilePointer,&ID3Tag);
        remainingBytes-=readedBytes;
        // printf("remaining: %d\n",remainingBytes);
        // remainingBytes = -1;
      }
      printTextFrame(*ID3Tag.TALB);
      printTextFrame(*ID3Tag.TPE1);
      printTextFrame(*ID3Tag.TPE2);
      printTextFrame(*ID3Tag.TCOM);
      printTextFrame(*ID3Tag.TDRC);
      printTextFrame(*ID3Tag.TPOS);
      printTextFrame(*ID3Tag.TCON);
      printTextFrame(*ID3Tag.TPE3);
      printTextFrame(*ID3Tag.TIT2);
      printTextFrame(*ID3Tag.TRCK);
      printTextFrame(*ID3Tag.TSSE);
    }

    fclose(mp3FilePointer);
    freeID3v2Tag(&ID3Tag);
    return(0);
  }
  else printf("The file DOESN'T exist!\n");

  return 0;
}


void printTextFrame(ID3v2TextFrameType frame){
  printf("\n----FRAME----\n");
  printf("Frame ID: %s\n",frame.header.frameId);
  printf("Flags: %u %u\n",frame.header.flags[0],frame.header.flags[1]);
  printf("Size: %u bytes\n",syncsafeToSize(frame.header.size));
  printf("TextEncoding: %d\n",frame.textEncoding);
  printf("Content: %s\n",frame.content);
}