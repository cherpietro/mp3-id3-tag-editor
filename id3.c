#include <stdio.h>
#include <stdint.h>
#include "ID3v2.h"


int main(){
  FILE *mp3FilePointer;
  // ID3v2HeaderType id3Header;
  // ID3v2DefaulFrameHeaderType id3FrameHeader;
  ID3TagType ID3Tag; 

  mp3FilePointer = fopen("./files/overture.mp3","r");
  if (mp3FilePointer) {
    if(readHeader(mp3FilePointer,&ID3Tag.header)){
      printf("Version: 2.%d.%d\n",ID3Tag.header.version[0],ID3Tag.header.version[1]);
      printf("Flag: %u\n",ID3Tag.header.flags);
      uint32_t tagSize = syncsafeToSize(ID3Tag.header.size);
      printf("Size: %u bytes\n",tagSize);
      
      // int readedBytes,remainingBytes;
      // remainingBytes = tagSize-10;
      // while(remainingBytes > 0){
      //   readedBytes = readFrame(mp3FilePointer,&ID3Tag.header);
      //   remainingBytes-=readedBytes;
      //   printf("remaining: %d\n",remainingBytes);
      // }
    }

    fclose(mp3FilePointer);
    return(0);
  }
  else printf("The file DOESN'T exist!\n");

  return 0;
}