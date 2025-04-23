#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "ID3v2.h"


void printTextFrame(ID3v2TextFrameType);
void printAPICFrame(ID3v2APICFrame frame);

int main(){
  FILE *mp3FilePointer;
  ID3TagType ID3Tag; 
  char *file = "./files/TimberHearth.mp3";
  // char *file = "./auxRemove.mp3";
  // char *file = "./files/jokin1.mp3";
  // char *file = "./files/theme.mp3";
  // char *file = "./files/overture.mp3";
  // char *file = "./files/gameFreak.mp3";
  mp3FilePointer = fopen(file,"r");
  if (mp3FilePointer) {
    ID3v2_init(&ID3Tag);

    ID3v2_storeTagInStruct(mp3FilePointer,&ID3Tag);
    ID3v2_getTagSizeOfTheStruct(&ID3Tag);
    // ID3v2_removeTagFromFile(mp3FilePointer,&ID3Tag);
    
    ID3v2_free(&ID3Tag);
    fclose(mp3FilePointer);
    return(0);
  }
  else printf("The file DOESN'T exist!\n");

  return 0;
}