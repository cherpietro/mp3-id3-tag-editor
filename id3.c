#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "ID3v2.h"


void storeTag(char *);

// void storeAndSaveAPIC(char *);
void listFrames(char *);

void printFrame(char *);

void modifyFrame(char *);

void storeAndWritte(char *);

void storeAndPrint(char *);



int main(){
  
  // char *file = "./files/auxFiles/jokin1.mp3"; //there are frames that aren't readed yet
  
  // char *file = "./files/EliteFour.mp3";
  // char *file = "./files/gameFreak.mp3";
  // char *file = "./files/jokin1.mp3";
  // char *file = "./files/jokin2.mp3"; //TEST TAG
  // char *file = "./files/jokin3.mp3";
  // char *file = "./files/nlp6.mp3";
  // char *file = "./files/opening.mp3";
  char *file = "./files/overture.mp3";
  // char *file = "./files/theme.mp3";
  // char *file = "./files/TimberHearth.mp3"; 
  // char *file = "./files/zelda.mp3";


  // char *file = "./files/ingenium/_NSYNC/No Strings Attached/NSYNC - Bye Bye Bye.mp3"; // ONE WOAS TAG
  // char *file = "./files/ingenium/(G)I-DLE/I feel/(G)I-DLE - Queencard.mp3"; // ONE WOAS TAG and USLT
  // char *file = "./files/ingenium/Alice Phoebe Lou/Glow/Alice Phoebe Lou - Only When I.mp3";
  // char *file = "./";
  storeTag(file);
  int option;
  int c;
  system("clear");
  do {
    printf("1. List Frames in TAG\n");
    printf("2. Print specific frame\n");
    printf("3. Modify Tag Frame\n");
    printf("6. Save changes in file\n");
 
    if (scanf("%d", &option) != 1) {
      system("clear");
      while ((c = getchar()) != '\n' && c != EOF);
      option = -1;
    }



    switch(option) {
        case 1:
            system("clear");
            listFrames(file);
            printf("\nPress enter to continue...\n");
            getchar();
            while ((c = getchar()) != '\n' && c != EOF);
            system("clear");
            break;
        case 2:
            system("clear");
            printFrame(file);
            printf("\nPress enter to continue...\n");
            getchar();
            while ((c = getchar()) != '\n' && c != EOF);
            system("clear");
            break;
        case 3:
            system("clear");
            modifyFrame(file);
            printf("\nPress enter to continue...\n");
            getchar();
            while ((c = getchar()) != '\n' && c != EOF);
            system("clear");
            break;
        case 4:
            printf("Option 4.\n");
            break;
        case 5:
            printf("Option 5.\n");
            break;
        case 6:
            // printf("\n");
            break;
        case 0:
            printf("Option 0.\n");
            break;
        default:
            printf("Not avaliable option\n\n");
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
  // int c;
  // while ((c = getchar()) != '\n' && c != EOF);
  ID3v2_printFrame(&ID3Tag, frameID);
  ID3v2_free(&ID3Tag);  
}

void modifyFrame(char *file){
  ID3TagType ID3Tag;
  ID3v2_init(&ID3Tag);
  ID3v2_storeTagInStruct(file,&ID3Tag);

  char frameID[5];
  printf("Introduce the Frame id to modify: ");
  scanf("%4s", frameID);
  printf("%s\n", frameID);
  int c;
  while ((c = getchar()) != '\n' && c != EOF);
  ID3v2_modifyFrame(&ID3Tag, frameID);
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

// void storeAndSaveAPIC(char *file){
//   ID3TagType ID3Tag;
//   ID3v2_init(&ID3Tag);
//   ID3v2_storeTagInStruct(file,&ID3Tag);
//   // ID3v2_saveAPICImage(&ID3Tag);
//   ID3v2_free(&ID3Tag);
// }

void storeAndWritte(char *file){
  ID3TagType ID3Tag;
  ID3v2_init(&ID3Tag);
  ID3v2_storeTagInStruct(file,&ID3Tag);
  ID3v2_writteTagIntoFile(file,&ID3Tag);
  ID3v2_free(&ID3Tag);
}