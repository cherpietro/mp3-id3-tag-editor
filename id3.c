#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "ID3v2.h"
#include "PrintFrameManager.h"
#include "FileFrameManager.h"

static void cleanInputBuffer(){
  int ch;
  while ((ch = getchar()) != '\n' && ch != EOF);
}

void saveChangesInFile(char *, ID3TagType);

int main(){
  
  // char *file = "./files/auxFiles/jokin1.mp3"; //there are frames that aren't readed yet
  
  // char *file = "./files/gameFreak.mp3";
  // char *file = "./files/jokin3.mp3";
  // char *file = "./files/nlp6.mp3";
  // char *file = "./files/opening.mp3";
  // char *file = "./files/theme.mp3";
  // char *file = "./files/zelda.mp3";
  // char *saveFile = "./files/auxFiles/overture.mp3";
  // char *file = "./files/jokin2.mp3"; //PCNT-APIC1-COMM1-TXT-TXXX8-PRIV8
  // char *file = "./files/jokin1.mp3"; //COMM1-APIC1-TXT-TXXX1-PRIV1
  // char *file = "./files/EliteFour.mp3"; //MDCI-TXT-APIC-PRIV(10)
  char *file = "./files/overture.mp3"; //APIC-TXT-TXXX6
  // char *file = "./files/TimberHearth.mp3"; //COMM2-TXT
  // char *file = "./files/ingenium/(G)I-DLE/I feel/(G)I-DLE - Queencard.mp3"; //WOAS - USLT

  system("clear");
  ID3TagType ID3Tag;
  ID3v2_init(&ID3Tag);
  ID3v2_storeTagInStruct(file,&ID3Tag);
  char frameID[5];

  int option;
  do {
    printf("1. List Frames in TAG\n");
    printf("2. Print specific frame\n");
    printf("3. Remove Tag Frame\n");
    printf("4. Add Tag Frame\n");
    printf("5. Save changes in file\n");
    printf("0. Exit\n");
 
    if (scanf("%d", &option) != 1) {
      system("clear");
      cleanInputBuffer();
      option = -1;
    }
    switch(option) {
        case 1:
            system("clear");
            PrintFrame_listFrames(&ID3Tag);

            printf("\nPress enter to continue...\n");
            getchar();
            cleanInputBuffer();
            system("clear");
            break;
        case 2:
            system("clear");          
            printf("Introduce the Frame id to print: ");
            scanf("%4s", frameID);
            PrintFrame_printFrame(&ID3Tag, frameID);

            printf("\nPress enter to continue...\n");
            getchar();
            cleanInputBuffer();
            system("clear");
            break;
        case 3:
            system("clear");
            printf("Introduce the Frame id to delete: ");
            scanf("%4s", frameID);
            printf("%s\n", frameID);
            cleanInputBuffer();
            ID3v2_deleteFrame(&ID3Tag, frameID);

            printf("\nPress enter to continue...\n");
            cleanInputBuffer();
            system("clear");
            break;
        case 4:
            system("clear");
            printf("Introduce the Frame id to include: ");
            scanf("%4s", frameID);
            printf("%s\n", frameID);
            cleanInputBuffer();
            ID3v2_addFrame(&ID3Tag, frameID);

            printf("\nPress enter to continue...\n");
            cleanInputBuffer();
            system("clear");
            break;
        case 5:
            saveChangesInFile(file,ID3Tag);
            break;
        case 0:
            printf("Option 0.\n");
            break;
        default:
            printf("Not avaliable option\n\n");
      }
  } while (option != 0);
  ID3v2_free(&ID3Tag);
  return 0;
}

void saveChangesInFile(char *file, ID3TagType ID3Tag){
  ID3v2_getTagSizeOfTheStruct(&ID3Tag);
  // FileManager_writteTagIntoFile(file,&ID3Tag);
  // printf("difference between files: \n");
  // system("cmp -l ./files/overture.mp3 ./savedFiles/modified.mp3 | wc -l");
  // printf("\n");
}