#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "ID3v2.h"

/*delete this */
#include "TXTFrameList.h"
#include "COMMFrameList.h"
#include "PRIVFrameList.h"
/**/

void printFrame(char *);

void storeTag(char *);

void listFrames(char *);

void storeAndSaveAPIC(char *);

void storeAndWritte(char *);

void storeAndPrint(char *);



int main(){
  
  // char *file = "./files/auxFiles/TimberHearth.mp3"; 
  
  // char *file = "./files/EliteFour.mp3";
  // char *file = "./files/gameFreak.mp3";
  // char *file = "./files/jokin1.mp3";
  // char *file = "./files/jokin2.mp3"; //TEST TAG
  // char *file = "./files/jokin3.mp3";
  // char *file = "./files/nlp6.mp3";
  // char *file = "./files/opening.mp3";
  // char *file = "./files/overture.mp3";
  // char *file = "./files/theme.mp3";
  // char *file = "./files/TimberHearth.mp3"; 
  // char *file = "./files/zelda.mp3";


  // char *file = "./files/ingenium/_NSYNC/No Strings Attached/NSYNC - Bye Bye Bye.mp3"; // ONE WOAS TAG
  // char *file = "./files/ingenium/(G)I-DLE/I feel/(G)I-DLE - Queencard.mp3"; // ONE WOAS TAG and USLT
  // char *file = "./files/ingenium/Alice Phoebe Lou/Glow/Alice Phoebe Lou - Only When I.mp3";
  // char *file = "./";
  // storeTag(file);

  // char *file = "./files/auxFiles/EliteFour.mp3"; 
  char *file = "./files/auxFiles/TimberHearth.mp3"; 
  // char *file = "./files/auxFiles/overture.mp3"; 
  storeTag(file);

  int option;
  do {
    printf("1. List Frames in TAG\n");
    printf("2. Print specific frame\n");
    printf("X. Modify Tag Frames\n");
    scanf("%d", &option);

    switch(option) {
        case 1:
            listFrames(file);
            break;
        case 2:
            printFrame(file);
            break;
        case 3:
            printf("Option 3.\n");
            break;
        case 4:
            printf("Option 4.\n");
            break;
        case 5:
            printf("Option 5.\n");
            break;
        case 0:
            printf("Option 0.\n");
            break;
        default:
            printf("Not avaliable option\n");
    }
} while (option != 0);
  // char *file = "./files/auxFiles/overture.mp3";
  // char *file = "./files/auxFiles/EliteFour.mp3";
  // storeAndSaveAPIC(file);
  // storeAndPrint(file);
  // storeAndWritte(file);
  return 0;
}

void printFrame(char *file){
  ID3TagType ID3Tag;
  ID3v2_init(&ID3Tag);
  ID3v2_storeTagInStruct(file,&ID3Tag);

  char frameID[5];
  printf("Introduce the Frame id to print: ");
  scanf("%4s", frameID);
  printf("%s\n", frameID);
  int c;
  while ((c = getchar()) != '\n' && c != EOF);
  ID3v2_printFrame(&ID3Tag, frameID);
  ID3v2_free(&ID3Tag);  
}

void listFrames(char *file){
  ID3TagType ID3Tag;
  ID3v2_init(&ID3Tag);
  ID3v2_storeTagInStruct(file,&ID3Tag);
  ID3v2_listFrames(&ID3Tag);
  ID3v2_free(&ID3Tag);
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