#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "ID3v2.h"


void printTextFrame(ID3v2TextFrameType);
void printAPICFrame(ID3v2APICFrame frame);

int main(){
  FILE *mp3FilePointer;
  ID3TagType ID3Tag; 

  mp3FilePointer = fopen("./files/EliteFour.mp3","r");
  if (mp3FilePointer) {
    readV2Tag(mp3FilePointer,&ID3Tag);
    fclose(mp3FilePointer);
    return(0);
  }
  else printf("The file DOESN'T exist!\n");

  return 0;
}