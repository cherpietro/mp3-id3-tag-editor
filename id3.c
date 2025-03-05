#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "ID3v2.h"

void printTextFrame(ID3v2TextFrameType);
void printAPICFrame(ID3v2APICFrame frame);

int main(){
  FILE *mp3FilePointer;
  ID3TagType ID3Tag; 
  initID3v2Tag(&ID3Tag);

  mp3FilePointer = fopen("./files/jokin1.mp3","r");
  if (mp3FilePointer) {
    if(readHeader(mp3FilePointer,&ID3Tag.header)){
      if(ID3Tag.header.version[0] == 4){
        printf("Version: 2.%d.%d\n",ID3Tag.header.version[0],ID3Tag.header.version[1]);
        printf("Flag: %u\n",ID3Tag.header.flags);
        uint32_t tagSize = syncsafeToSize(ID3Tag.header.size);
        printf("Size: %u bytes\n",tagSize);
        int readedBytes,remainingBytes,totalReaded;
        totalReaded = 0;
        remainingBytes = tagSize-10;
        while(remainingBytes > 0){
          readedBytes = readFrame(mp3FilePointer,&ID3Tag);
          remainingBytes-=readedBytes;
          totalReaded+=readedBytes;
          printf("remaining: %d\n",remainingBytes);
          // remainingBytes = -1;
        }
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
        printf("\nFinal remaining Bytes: %d\n",remainingBytes);
      }
      else if(ID3Tag.header.version[0] == 3){
        printf("Version: 2.%d.%d\n",ID3Tag.header.version[0],ID3Tag.header.version[1]);
        printf("Flag: %u\n",ID3Tag.header.flags);
        uint32_t tagSize = syncsafeToSize(ID3Tag.header.size);
        printf("Size: %u bytes\n",tagSize);
        int remainingBytes,totalReaded;
        totalReaded = 0;
        remainingBytes = tagSize-10;
        int paddingReached = 0;
        while(ftell(mp3FilePointer) < tagSize + 10 && paddingReached != 1){
          paddingReached = readFramev2_3(mp3FilePointer,&ID3Tag);
        }
        printf("\nSuposed Size: %u bytes\n",tagSize);
        printf("\nFinal remaining Bytes: %d\n",remainingBytes);
        printf("\nFinal readed Bytes: %d\n",totalReaded);
      }
      else{
        printf("Not yet supported tag version\n");
      }
      
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