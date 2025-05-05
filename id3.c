#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>

#include "ID3v2.h"
#include "FrameManager/PrintFrameManager.h"
#include "FrameManager/FileFrameManager.h"

static void cleanInputBuffer(){
  int ch;
  while ((ch = getchar()) != '\n' && ch != EOF);
}

void saveChangesInFile(char *, ID3TagType *);
void removeTagInFile(char *);
void printFrameDefinition();

char* openDir(){
    DIR *dir;
    struct dirent *entry;
    char *dir_path = "./files";
    int count = 0;
    int position;
    static char full_path[1024]; 

    system("clear");
    dir = opendir(dir_path);
    if (dir == NULL) {
        printf("Error opening directory '%s'\n",dir_path);
        return NULL;
    }
    printf("Files in directory '%s':\n", dir_path);
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) { 
            count++;
            printf("%d. %s\n", count, entry->d_name);
        }
    }
    closedir(dir);  

    do{
      printf("Select the file to open (0 to exit): ");
      if (scanf("%d", &position) != 1) {
        cleanInputBuffer();
        position = -1;
      }
    }while(position < 0 || position > count);
    if(position == 0) return NULL;
    
    dir = opendir(dir_path);
    if (dir == NULL) {
      printf("Error opening directory '%s'\n",dir_path);
      return NULL;
    }

    count = 0;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) {
            count++;
            if (count == position) {
                snprintf(full_path, sizeof(full_path), "%s/%s", dir_path, entry->d_name);
                break;
            }
        }
    }
    system("clear");
    closedir(dir);  
    return full_path;
}
int main(){
    // char *file = "./files/jokin2.mp3"; //PCNT-APIC1-COMM1-TXT-TXXX8-PRIV8
    // char *file = "./files/jokin1.mp3"; //COMM1-APIC1-TXT-TXXX1-PRIV1
    // char *file = "./files/EliteFour.mp3"; //MDCI-TXT-APIC-PRIV(10)
    // char *file = "./files/overture.mp3"; //APIC-TXT-TXXX6
    // char *file = "./files/TimberHearth.mp3"; //COMM2-TXT
    // char *file = "./files/ingenium/(G)I-DLE/I feel/(G)I-DLE - Queencard.mp3"; //WOAS - USLT
    // char *file = "./files/void.mp3";
    char *file = openDir();
    if(file == NULL) return 0;
    system("clear");
    ID3TagType ID3Tag;
    ID3v2_init(&ID3Tag);
    int status = ID3v2_storeTagInStruct(file,&ID3Tag);
    if(status == -1 || status == -2) return 0;
    char frameID[5];
    int option;
    do {
        HeaderV2_printTagHeader(ID3Tag.header);
        printf("1. List Frames in TAG\n");
        printf("2. Print specific frame\n");
        printf("3. Remove Tag Frame\n");
        printf("4. Add Tag Frame\n");
        printf("5. Save changes in file\n");
        printf("6. Remove Tag from file\n");
        printf("7. Print whole tag\n");
        printf("8. Show supported frames\n");
        printf("\n0. Exit (Remember to save tag into file)\n");
        printf("\n\nSelect option: ");
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
                cleanInputBuffer();
                ID3v2_addFrame(&ID3Tag, frameID);

                printf("\nPress enter to continue...\n");
                cleanInputBuffer();
                system("clear");
                break;
            case 5:
                system("clear");          
                saveChangesInFile(file,&ID3Tag);
                cleanInputBuffer();
                printf("\nPress enter to continue...\n");
                cleanInputBuffer();
                system("clear");
                break;
            case 6:
                system("clear");  
                removeTagInFile(file);
                cleanInputBuffer();
                printf("\nPress enter to continue...\n");
                cleanInputBuffer();
                system("clear");
                break;
            case 7:
                system("clear");  
                printTag(&ID3Tag);
                cleanInputBuffer();
                printf("\nPress enter to continue...\n");
                cleanInputBuffer();
                system("clear");
                break;
            case 8:
                system("clear");  
                printFrameDefinition();
                cleanInputBuffer();
                printf("\nPress enter to continue...\n");
                cleanInputBuffer();
                system("clear");
                break;
            case 0:
                printf("Good bye :)\n");
                break;
            default:
                printf("Not avaliable option\n\n");
        }
    } while (option != 0);
    ID3v2_free(&ID3Tag);
    return 0;
}

void saveChangesInFile(char *file, ID3TagType *ID3Tag){
    if(!ID3v2_getTagSizeOfTheStruct(ID3Tag)){
        printf("error with size of the the tag\n");
        return;
    }
    //Check this function return
    FileManager_writteTagIntoFile(file,ID3Tag);
  // printf("difference between files: \n");
  // system("cmp -l ./files/overture.mp3 ./savedFiles/modified.mp3 | wc -l");
  // printf("\n");
}
void removeTagInFile(char *file){
    if(FileManager_removeTagFromFile(file)) printf("File saved in ./savedFiles/tagRemoved.mp3\n");
    else printf("Error removing tag\n");
}

void printFrameDefinition() {
    char *file = "./FrameDefinition.txt";
    FILE *filePtr = fopen(file, "r");
    if (filePtr == NULL) {
        printf("Error opening file");
        return;
    }

    char linea[256];
    while (fgets(linea, sizeof(linea), filePtr)) {
        printf("%s", linea);
    }
    printf("\n\n");
    fclose(filePtr);
}
