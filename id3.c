#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "ID3v2.h"


void storeAndSaveAPIC(char *);

void storeAndWritte(char *);

void storeAndPrint(char *);

void testOverture();

void testTimberHearth();

int main(){
  
  // char *file = "./files/EliteFour.mp3"; //there are frames that aren't readed yet
  // char *file = "./files/jokin1.mp3"; //there are frames that aren't readed yet
  
  // char *file = "./files/theme.mp3";
  // char *file = "./files/nlp6.mp3";
  // char *file = "./files/gameFreak.mp3";
  char *file = "./files/auxFiles/overture.mp3";
  storeAndSaveAPIC(file);
  // storeAndPrint(file);
  // storeAndWritte(file);
  
  // testOverture();
  // testTimberHearth();
  return 0;
}

void storeAndPrint(char *file){
  ID3TagType ID3Tag;
  ID3v2_init(&ID3Tag);
  ID3v2_storeTagInStruct(file,&ID3Tag);
  printTag(&ID3Tag);
  ID3v2_free(&ID3Tag);
}

void storeAndSaveAPIC(char *file){
  ID3TagType ID3Tag;
  ID3v2_init(&ID3Tag);
  ID3v2_storeTagInStruct(file,&ID3Tag);
  ID3v2_saveAPICImage(&ID3Tag);
  ID3v2_free(&ID3Tag);
}

void storeAndWritte(char *file){
  ID3TagType ID3Tag;
  ID3v2_init(&ID3Tag);
  ID3v2_storeTagInStruct(file,&ID3Tag);
  ID3v2_writteTagIntoFile(file,&ID3Tag);
  ID3v2_free(&ID3Tag);
}

void testOverture(){
  ID3TagType ID3Tag;
  char *file = "./files/auxFiles/overture.mp3";
  ID3v2_init(&ID3Tag);
  ID3v2_storeTagInStruct(file,&ID3Tag);
  ID3v2_writteTagIntoFile(file,&ID3Tag);
  ID3v2_free(&ID3Tag);
  printf("Difference between files: \n");
  system("cmp -l ./files/overture.mp3 ./files/auxFiles/overture.mp3 | wc -l");
}

void testTimberHearth(){
  ID3TagType ID3Tag;
  char *file = "./files/auxFiles/TimberHearth.mp3";
  ID3v2_init(&ID3Tag);
  ID3v2_storeTagInStruct(file,&ID3Tag);
  TIMBERHEARTH_ID3v2_writteTagIntoFile(file,&ID3Tag);
  ID3v2_free(&ID3Tag);
  
  printf("Difference between files: \n");
  system("cmp -l ./files/TimberHearth.mp3 ./files/auxFiles/TimberHearth.mp3 | wc -l");
}