#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <byteswap.h>
#include "ID3v2.h"


int main(){
  FILE *mp3FilePointer;
  ID3v2HeaderType id3Header;
  ID3v2DefaulFrameHeaderType id3FrameHeader;

  mp3FilePointer = fopen("./files/theme.mp3","r");
  if (mp3FilePointer) {
    if(readHeader(mp3FilePointer,&id3Header)){
      printf("Version: 2.%d.%d\n",id3Header.version[0],id3Header.version[1]);
      printf("Flag: %u\n",id3Header.flags);
      uint32_t tagSize = syncsafeToSize(id3Header.size);
      printf("Size: %u bytes\n",tagSize);
      
      int readedBytes,remainingBytes;
      remainingBytes = tagSize-10;
      while(remainingBytes > 0){
        readedBytes = readFrame(mp3FilePointer,&id3FrameHeader);
        remainingBytes-=readedBytes;
        // printf("remaining: %d\n",remainingBytes);
      }
    }

    fclose(mp3FilePointer);
    return(0);
  }
  else printf("The file DOESN'T exist!\n");

  return 0;
}