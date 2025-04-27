#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "ID3v2.h"


void storeTag(char *);

void storeAndSaveAPIC(char *);

void storeAndWritte(char *);

void storeAndPrint(char *);



int main(){
  
  // char *file = "./files/EliteFour.mp3"; //there are frames that aren't readed yet
  // char *file = "./files/auxFiles/jokin1.mp3"; //there are frames that aren't readed yet
  char *file = "./files/jokin1.mp3"; //there are frames that aren't readed yet
  
  // char *file = "./files/theme.mp3";
  // char *file = "./files/nlp6.mp3";
  // char *file = "./files/gameFreak.mp3";
  // char *file = "./files/jokin3.mp3"; 
  storeTag(file);
  
  
  // char *file = "./files/auxFiles/overture.mp3";
  // char *file = "./files/auxFiles/EliteFour.mp3";
  // storeAndSaveAPIC(file);
  // storeAndPrint(file);
  // storeAndWritte(file);
  return 0;
}

void storeTag(char *file){
  ID3TagType ID3Tag;
  ID3v2_init(&ID3Tag);
  ID3v2_storeTagInStruct(file,&ID3Tag);
  ID3v2_free(&ID3Tag);
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