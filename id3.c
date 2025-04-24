#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "ID3v2.h"

int main(){
  FILE *mp3FilePointer;
  ID3TagType ID3Tag; 
  // char *file = "./files/EliteFour.mp3"; //there are frames that aren't readed yet
  // char *file = "./files/jokin1.mp3"; //there are frames that aren't readed yet
  // char *file = "./auxRemove.mp3";
  // char *file = "./files/TimberHearth.mp3";
  char *file = "./aux.mp3";
  char *fileToWrite = "./tagRemoved.mp3";
  // char *file = "./files/theme.mp3";
  // char *file = "./files/overture.mp3";
  // char *file = "./files/gameFreak.mp3";
  mp3FilePointer = fopen(file,"r");
  if (mp3FilePointer) {
    ID3v2_init(&ID3Tag);
    
    ID3v2_storeTagInStruct(mp3FilePointer,&ID3Tag);
    ID3v2_getTagSizeOfTheStruct(&ID3Tag);
    ID3v2_removeTagFromFile(mp3FilePointer);
    FILE * fileToWritePtr = fopen(fileToWrite,"r");
    ID3v2_writteTagIntoFile(fileToWritePtr,&ID3Tag);
    ID3v2_free(&ID3Tag);
    fclose(mp3FilePointer);
    // fclose(fileToWritePtr);
    return(0);
  }
  else printf("The file DOESN'T exist!\n");

  return 0;
}