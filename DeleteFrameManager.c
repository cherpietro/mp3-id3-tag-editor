#include "DeleteFrameManager.h"
#include <string.h>

static void cleanInputBuffer(){
  int ch;
  while ((ch = getchar()) != '\n' && ch != EOF);
}

void DeleteFrame_deleteTXXX(ListFramePtr *TXTFrameList){
  int option;
  ListFramePtr_setFirstActive(TXTFrameList);
    ID3v2TXTFrameType * TXTFramePtr = ListFramePtr_getActiveFramePtr(*TXTFrameList);
    do{
      if(strncasecmp(TXTFramePtr->header.frameId,"TXXX",4)==0){
        fflush(stdout);
        system("clear");
        FramesV2_printTXTF(*TXTFramePtr);
        printf("\n\nWant to delete this frame? (y/n): ");
        option = getchar();
        cleanInputBuffer();
        if(option == 'y')  ListFramePtr_deleteActive(TXTFrameList);
        else ListFramePtr_setNextActive(TXTFrameList);
      }
      else ListFramePtr_setNextActive(TXTFrameList);
      TXTFramePtr = ListFramePtr_getActiveFramePtr(*TXTFrameList);
    }while (TXTFramePtr != NULL);
}

void DeleteFrame_deleteTXTF(ListFramePtr *TXTFrameList, char *frameID){
  char option;
  ListFramePtr_setFirstActive(TXTFrameList);
    ID3v2TXTFrameType * TXTFramePtr = ListFramePtr_getActiveFramePtr(*TXTFrameList);
    while (TXTFramePtr != NULL && strncasecmp(frameID,TXTFramePtr->header.frameId,4) != 0){
      ListFramePtr_setNextActive(TXTFrameList);
      TXTFramePtr = ListFramePtr_getActiveFramePtr(*TXTFrameList);
    }
    if(TXTFramePtr != NULL){
      fflush(stdout);
      system("clear");
      FramesV2_printTXTF(*TXTFramePtr);
      printf("\n\nWant to delete this frame? (y/n): ");
      option = getchar();
      cleanInputBuffer();
      if(option == 'y') ListFramePtr_deleteActive(TXTFrameList);
    } 
}

void DeleteFrame_deletePRIV(ListFramePtr *PRIVFrameList){
  char option;
  ListFramePtr_setFirstActive(PRIVFrameList);
  ID3v2PRIVFrameType * PRIVFramePtr = ListFramePtr_getActiveFramePtr(*PRIVFrameList);
  do{
    fflush(stdout);
    system("clear");
    FramesV2_printPRIV(*PRIVFramePtr);
    printf("\n\nWant to delete this frame? (y/n): ");
    option = getchar();
    cleanInputBuffer();
    if(option == 'y') ListFramePtr_deleteActive(PRIVFrameList);
    else ListFramePtr_setNextActive(PRIVFrameList);
    PRIVFramePtr = ListFramePtr_getActiveFramePtr(*PRIVFrameList);
  }while (PRIVFramePtr != NULL);
}

void DeleteFrame_deleteCOMM(ListFramePtr *COMMFrameList){
  char option;  
  ListFramePtr_setFirstActive(COMMFrameList);
  ID3v2COMMFrameType * COMMFramePtr = ListFramePtr_getActiveFramePtr(*COMMFrameList);
  do{
    fflush(stdout);
    system("clear");
    FramesV2_printCOMM(*COMMFramePtr);
    printf("\n\nWant to delete this frame? (y/n): ");
    option = getchar();
    cleanInputBuffer();
    if(option == 'y') ListFramePtr_deleteActive(COMMFrameList);
    else ListFramePtr_setNextActive(COMMFrameList);
    COMMFramePtr = ListFramePtr_getActiveFramePtr(*COMMFrameList);
  }while (COMMFramePtr != NULL);
}
void DeleteFrame_deleteAPIC(ListFramePtr *APICFrameList){
  char option;
  ListFramePtr_setFirstActive(APICFrameList);
  ID3v2APICFrameType * APICFramePtr = ListFramePtr_getActiveFramePtr(*APICFrameList);
  do{
    fflush(stdout);
    system("clear");
    FramesV2_printAPIC(*APICFramePtr);
    printf("\n\nWant to delete this frame? (y/n): ");
    option = getchar();
    cleanInputBuffer();
    if(option == 'y') {
      ListFramePtr_deleteActive(APICFrameList);
    }
    else{
      ListFramePtr_setNextActive(APICFrameList);
    }
    APICFramePtr = ListFramePtr_getActiveFramePtr(*APICFrameList);
  }while (APICFramePtr != NULL);
}
void DeleteFrame_deletePOPM(ListFramePtr *POPMFrameList){
  char option;
  ListFramePtr_setFirstActive(POPMFrameList);
  ID3v2POPMFrameType * POPMFramePtr = ListFramePtr_getActiveFramePtr(*POPMFrameList);
  do{
    fflush(stdout);
    system("clear");
    FramesV2_printPOPM(*POPMFramePtr);
    printf("\n\nWant to delete this frame? (y/n): ");
    option = getchar();
    cleanInputBuffer();
    if(option == 'y') {
      ListFramePtr_deleteActive(POPMFrameList);
    }
    else{
      ListFramePtr_setNextActive(POPMFrameList);
    }
    POPMFramePtr = ListFramePtr_getActiveFramePtr(*POPMFrameList);
  }while (POPMFramePtr != NULL);
}