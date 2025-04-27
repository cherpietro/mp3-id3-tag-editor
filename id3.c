#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "ID3v2.h"


void storeRemoveAndWritte(FILE *,ID3TagType *);

void testTimberHearth();

int main(){
  
  // char *file = "./files/EliteFour.mp3"; //there are frames that aren't readed yet
  // char *file = "./files/jokin1.mp3"; //there are frames that aren't readed yet
  
  // char *file = "./files/theme.mp3";
  // char *file = "./files/nlp6.mp3";
  // char *file = "./files/overture.mp3";
  // char *file = "./files/gameFreak.mp3";
  testTimberHearth();
  return 0;
}

void storeRemoveAndWritte(FILE *mp3FilePointer,ID3TagType *ID3Tag){
  char *fileToWrite = "./tagRemoved.mp3";
  FILE * fileToWritePtr = fopen(fileToWrite,"r");

  ID3v2_storeTagInStruct(mp3FilePointer,ID3Tag);
  ID3v2_getTagSizeOfTheStruct(ID3Tag);
  ID3v2_removeTagFromFile(mp3FilePointer);

  ID3v2_writteTagIntoFile(fileToWritePtr,ID3Tag);
  fclose(fileToWritePtr);
}

void testTimberHearth(){
  FILE *mp3FilePointer;
  ID3TagType ID3Tag;
  char *file = "./files/TimberHearth.mp3";
  mp3FilePointer = fopen(file,"r");
  if (mp3FilePointer) {
    ID3v2_init(&ID3Tag);
    ID3v2_storeTagInStruct(mp3FilePointer,&ID3Tag);
    ID3v2_removeTagFromFile(mp3FilePointer);
    fclose(mp3FilePointer);

    char *fileToWrite = "./tagRemoved.mp3";
    FILE * fileToWritePtr = fopen(fileToWrite,"r");
    TIMBERHEARTH_ID3v2_writteTagIntoFile(fileToWritePtr,&ID3Tag);
    ID3v2_free(&ID3Tag);
    printf("Difference between files: \n");
    system("cmp -l ./files/TimberHearth.mp3 ./temp.mp3 | wc -l");
  }
  else {
    printf("The file DOESN'T exist!\n");
  }
  
}