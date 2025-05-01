#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "ID3v2.h"

static void cleanInputBuffer(){
  int ch;
  while ((ch = getchar()) != '\n' && ch != EOF);
}
void storeTag(char *);

// void storeAndSaveAPIC(char *);
void listFrames(char *);

void printFrame(char *);

void modifyFrame(char *);

void storeAndWritte(char *);

void storeAndPrint(char *);

void saveChangesInFile(char *, ID3TagType);

int main(){
  
  // char *file = "./files/auxFiles/jokin1.mp3"; //there are frames that aren't readed yet
  
  // char *file = "./files/EliteFour.mp3";
  // char *file = "./files/gameFreak.mp3";
  // char *file = "./files/jokin1.mp3";
  char *file = "./files/jokin2.mp3"; //TEST TAG
  // char *file = "./files/jokin3.mp3";
  // char *file = "./files/nlp6.mp3";
  // char *file = "./files/opening.mp3";
  // char *file = "./files/overture.mp3";
  // char *saveFile = "./files/auxFiles/overture.mp3";
  // char *file = "./files/theme.mp3";
  // char *file = "./files/TimberHearth.mp3"; 
  // char *file = "./files/zelda.mp3";


  // char *file = "./files/ingenium/_NSYNC/No Strings Attached/NSYNC - Bye Bye Bye.mp3"; // ONE WOAS TAG
  // char *file = "./files/ingenium/(G)I-DLE/I feel/(G)I-DLE - Queencard.mp3"; // ONE WOAS TAG and USLT
  // char *file = "./files/ingenium/Alice Phoebe Lou/Glow/Alice Phoebe Lou - Only When I.mp3";
  // char *file = "./";
  // storeTag(file);

  system("clear");
  ID3TagType ID3Tag;
  ID3v2_init(&ID3Tag);
  ID3v2_storeTagInStruct(file,&ID3Tag);
  char frameID[5];

  int option;
  do {
    printf("1. List Frames in TAG\n");
    printf("2. Print specific frame\n");
    printf("3. Modify Tag Frame\n");
    printf("4. Remove Tag Frame\n");
    printf("5. Add Tag Frame\n");
    printf("6. Save changes in file\n");
    printf("0. Exit\n");
 
    if (scanf("%d", &option) != 1) {
      system("clear");
      cleanInputBuffer();
      option = -1;
    }



    switch(option) {
        case 1:
            system("clear");
            // listFrames(file);
            ID3v2_listFrames(&ID3Tag);
            printf("\nPress enter to continue...\n");
            getchar();
            cleanInputBuffer();
            system("clear");
            break;
        case 2:
            system("clear");
            // printFrame(file)
            
            printf("Introduce the Frame id to print: ");
            scanf("%4s", frameID);
            ID3v2_printFrame(&ID3Tag, frameID);

            printf("\nPress enter to continue...\n");
            getchar();
            cleanInputBuffer();
            system("clear");
            break;
        case 3:
            system("clear");
            // modifyFrame(file);
            printf("Introduce the Frame id to modify: ");
            scanf("%4s", frameID);
            printf("%s\n", frameID);
            cleanInputBuffer();
            // ID3v2_modifyFrame(&ID3Tag, frameID);

            printf("\nPress enter to continue...\n");
            cleanInputBuffer();
            system("clear");
            break;
        case 4:
            system("clear");
            // modifyFrame(file);
            printf("Introduce the Frame id to delete: ");
            scanf("%4s", frameID);
            printf("%s\n", frameID);
            cleanInputBuffer();
            ID3v2_deleteFrame(&ID3Tag, frameID);

            printf("\nPress enter to continue...\n");
            cleanInputBuffer();
            system("clear");
            break;
        case 5:
            system("clear");
            // modifyFrame(file);
            printf("Introduce the Frame id to implement: ");
            scanf("%4s", frameID);
            printf("%s\n", frameID);
            cleanInputBuffer();
            ID3v2_addFrame(&ID3Tag, frameID);

            printf("\nPress enter to continue...\n");
            cleanInputBuffer();
            system("clear");
            break;
        case 6:
            saveChangesInFile(file,ID3Tag);

            // printf("\n");
            break;
        case 0:
            printf("Option 0.\n");
            break;
        default:
            printf("Not avaliable option\n\n");
      }
  } while (option != 0);
  ID3v2_free(&ID3Tag);
  // char *file = "./files/auxFiles/overture.mp3";
  // char *file = "./files/auxFiles/EliteFour.mp3";
  // storeAndSaveAPIC(file);
  // storeAndPrint(file);
  // storeAndWritte(file);
  return 0;
}

void saveChangesInFile(char *file, ID3TagType ID3Tag){
  ID3v2_writteTagIntoFile(file,&ID3Tag);
  printf("difference between files: \n");
  system("cmp -l ./files/overture.mp3 ./savedFiles/modified.mp3 | wc -l");
  printf("\n");
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
  // ID3v2_modifyFrame(&ID3Tag, frameID);
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