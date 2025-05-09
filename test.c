#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "ID3v2.h"

void testOverture();

void testTimberHearth();

void testJokin3();

void testEliteFour();

void testJokin1();

void testJokin1_FileManager_writteTagIntoFile(char *, ID3TagType *);

void TIMBERHEARTH_FileManager_writteTagIntoFile(char *, ID3TagType *);

void JOKIN3_FileManager_writteTagIntoFile(char *, ID3TagType *);

void ELITEFOUR_FileManager_writteTagIntoFile(char *, ID3TagType *);
int main(){
  testJokin1();
  testEliteFour();
  testOverture();
  testTimberHearth();
  testJokin3();
}
void testJokin1(){
  ID3TagType ID3Tag;
  char *file = "./files/auxFiles/jokin1.mp3";
  ID3v2_init(&ID3Tag);
  ID3v2_storeTagInStruct(file,&ID3Tag);
  testJokin1_FileManager_writteTagIntoFile(file,&ID3Tag);
  ID3v2_free(&ID3Tag);
  printf("Difference between files: \n");
  system("cmp -l ./files/jokin1.mp3 ./files/auxFiles/jokin1.mp3 | wc -l");
}

void testOverture(){
  ID3TagType ID3Tag;
  char *file = "./files/auxFiles/overture.mp3";
  ID3v2_init(&ID3Tag);
  ID3v2_storeTagInStruct(file,&ID3Tag);
  FileManager_writteTagIntoFile(file,&ID3Tag);
  ID3v2_free(&ID3Tag);
  printf("Difference between files: \n");
  system("cmp -l ./files/overture.mp3 ./files/auxFiles/overture.mp3 | wc -l");
}

void testEliteFour(){
  ID3TagType ID3Tag;
  char *file = "./files/auxFiles/EliteFour.mp3";  
  ID3v2_init(&ID3Tag);
  ID3v2_storeTagInStruct(file,&ID3Tag);
  ELITEFOUR_FileManager_writteTagIntoFile(file,&ID3Tag);
  ID3v2_free(&ID3Tag);
  
  printf("Difference between files: \n");
  system("cmp -l ./files/EliteFour.mp3 ./files/auxFiles/EliteFour.mp3 | wc -l");
}
void testTimberHearth(){
  ID3TagType ID3Tag;
  char *file = "./files/auxFiles/TimberHearth.mp3";
  ID3v2_init(&ID3Tag);
  ID3v2_storeTagInStruct(file,&ID3Tag);
  TIMBERHEARTH_FileManager_writteTagIntoFile(file,&ID3Tag);
  ID3v2_free(&ID3Tag);
  
  printf("Difference between files: \n");
  system("cmp -l ./files/TimberHearth.mp3 ./files/auxFiles/TimberHearth.mp3 | wc -l");
}

void testJokin3(){
  ID3TagType ID3Tag;
  char *file = "./files/auxFiles/jokin3.mp3";
  ID3v2_init(&ID3Tag);
  ID3v2_storeTagInStruct(file,&ID3Tag);
  JOKIN3_FileManager_writteTagIntoFile(file,&ID3Tag);
  ID3v2_free(&ID3Tag);
  
  printf("Difference between files: \n");
  system("cmp -l ./files/jokin3.mp3 ./files/auxFiles/jokin3.mp3 | wc -l");
}

void TIMBERHEARTH_FileManager_writteTagIntoFile(char *file, ID3TagType *ID3Tag){
  FileManager_removeTagFromFile(file);

  FILE *mp3FilePointer = fopen(file,"r");
  if(mp3FilePointer){
    fseek(mp3FilePointer,0,SEEK_END);
    uint32_t fileSize = ftell(mp3FilePointer);
    fseek(mp3FilePointer,0,SEEK_SET);
    unsigned char *dataBuffer = (unsigned char *)malloc(fileSize);
    if(!dataBuffer){
      printf("error");
      fclose(mp3FilePointer);
      return;
    }
    fread(dataBuffer,1,fileSize,mp3FilePointer);
    fclose(mp3FilePointer);

    FILE *temp = fopen("temp.mp3","w");
    if(!temp){
      printf("error");
      fclose(temp);
      return;
    }
    // header
    fwrite(&ID3Tag->header,1,sizeof(ID3Tag->header),temp);
    /*AUX*/
    ID3v2TXTFrameType *TXTFrame;
    ListFramePtr_setFirstActive(&ID3Tag->TXTFrameList);
    TXTFrame = (ID3v2TXTFrameType *)ListFramePtr_getActiveFramePtr(ID3Tag->TXTFrameList);
    fwrite(&TXTFrame->header,1, sizeof(TXTFrame->header),temp);
    fwrite(&TXTFrame->textEncoding,1, 1,temp);
    fwrite(TXTFrame->content.string,1, TXTFrame->content.size,temp);
    ListFramePtr_setNextActive(&ID3Tag->TXTFrameList);
    /**/
    //COMMFrames
    ID3v2COMMFrameType *COMMFrame;
    ListFramePtr_setFirstActive(&ID3Tag->COMMFrameList);
    while(ID3Tag->COMMFrameList.active != NULL){
      COMMFrame = (ID3v2COMMFrameType *)ListFramePtr_getActiveFramePtr(ID3Tag->COMMFrameList);
      fwrite(&COMMFrame->header,1, sizeof(COMMFrame->header),temp);
      fwrite(&COMMFrame->textEncoding,1, 1,temp);
      fwrite(&COMMFrame->language,1, 3,temp);
      fwrite(COMMFrame->contentDescript.string,1, TxtStr_getStringLen(COMMFrame->contentDescript),temp);
      fwrite(COMMFrame->actualText.string,1, TxtStr_getStringLen(COMMFrame->actualText),temp);
      ListFramePtr_setNextActive(&ID3Tag->COMMFrameList);
    }
    while(ID3Tag->TXTFrameList.active != NULL){
      TXTFrame = (ID3v2TXTFrameType *)ListFramePtr_getActiveFramePtr(ID3Tag->TXTFrameList);
      fwrite(&TXTFrame->header,1, sizeof(TXTFrame->header),temp);
      fwrite(&TXTFrame->textEncoding,1, 1,temp);
      fwrite(TXTFrame->content.string,1, TxtStr_getStringLen(TXTFrame->content),temp);
      ListFramePtr_setNextActive(&ID3Tag->TXTFrameList);
    }
    char zero = 0;
    for (int i = 0; i < (int) ID3Tag->paddingSize; i++) {
        fwrite(&zero, 1, 1, temp);
    }
    // content
    fwrite(dataBuffer,1,fileSize,temp);
    fclose(temp);

    remove(file);
    rename("temp.mp3",file);
  }
}

void JOKIN3_FileManager_writteTagIntoFile(char *file, ID3TagType *ID3Tag){
  FileManager_removeTagFromFile(file);
  FILE *mp3FilePointer = fopen(file,"r");
  if(mp3FilePointer){
    fseek(mp3FilePointer,0,SEEK_END);
    uint32_t fileSize = ftell(mp3FilePointer);
    fseek(mp3FilePointer,0,SEEK_SET);
    unsigned char *dataBuffer = (unsigned char *)malloc(fileSize);
    if(!dataBuffer){
      printf("error");
      fclose(mp3FilePointer);
      return;
    }
    fread(dataBuffer,1,fileSize,mp3FilePointer);
    FILE *temp = fopen("temp.mp3","w");
    if(!temp){
      printf("error");
      fclose(temp);
      return;
    }
    // header
    fwrite(&ID3Tag->header,1,sizeof(ID3Tag->header),temp);
     //TXTFrames
     ID3v2TXTFrameType *TXTFrame;
     ListFramePtr_setFirstActive(&ID3Tag->TXTFrameList);
     while(ID3Tag->TXTFrameList.active != NULL){
       TXTFrame = (ID3v2TXTFrameType *)ListFramePtr_getActiveFramePtr(ID3Tag->TXTFrameList);
       fwrite(&TXTFrame->header,1, sizeof(TXTFrame->header),temp);
       fwrite(&TXTFrame->textEncoding,1, 1,temp);
       fwrite(TXTFrame->content.string,1, TxtStr_getStringLen(TXTFrame->content),temp);
       ListFramePtr_setNextActive(&ID3Tag->TXTFrameList);
     }
     
    //APIC
    ID3v2APICFrameType *APICFrame;
    ListFramePtr_setFirstActive(&ID3Tag->APICFrameList);
    while(ID3Tag->APICFrameList.active != NULL){
      APICFrame = (ID3v2APICFrameType *)ListFramePtr_getActiveFramePtr(ID3Tag->APICFrameList);
      fwrite(&APICFrame->header,1, sizeof(APICFrame->header),temp);
      fwrite(&APICFrame->textEncoding,1, 1,temp);
      fwrite(APICFrame->mimeType.string,1, TxtStr_getStringLen(APICFrame->mimeType),temp);
      fwrite(&APICFrame->pictureType,1, 1,temp);
      fwrite(APICFrame->description.string,1, TxtStr_getStringLen(APICFrame->description),temp);
      fwrite(APICFrame->imageData,1,APICFrame->imageDataSize ,temp);
      ListFramePtr_setNextActive(&ID3Tag->APICFrameList);
    }

    //COMMFrames
    ID3v2COMMFrameType *COMMFrame;
    ListFramePtr_setFirstActive(&ID3Tag->COMMFrameList);
    while(ID3Tag->COMMFrameList.active != NULL){
      COMMFrame = (ID3v2COMMFrameType *) ListFramePtr_getActiveFramePtr(ID3Tag->COMMFrameList);
      fwrite(&COMMFrame->header,1, sizeof(COMMFrame->header),temp);
      fwrite(&COMMFrame->textEncoding,1, 1,temp);
      fwrite(&COMMFrame->language,1, 3,temp);
      fwrite(COMMFrame->contentDescript.string,1, TxtStr_getStringLen(COMMFrame->contentDescript),temp);
      fwrite(COMMFrame->actualText.string,1, TxtStr_getStringLen(COMMFrame->actualText),temp);
      ListFramePtr_setNextActive(&ID3Tag->COMMFrameList);
    }
    
    //Padding
    char zero = 0;
    for (int i = 0; i < (int) ID3Tag->paddingSize; i++) {
        fwrite(&zero, 1, 1, temp);
    }

    // content
    fwrite(dataBuffer,1,fileSize,temp);
    fclose(temp);

    remove(file);
    rename("temp.mp3",file);
  }
}
void testJokin1_FileManager_writteTagIntoFile(char *file, ID3TagType *ID3Tag){
  FileManager_removeTagFromFile(file);
  FILE *mp3FilePointer = fopen(file,"r");
  if(mp3FilePointer){
    fseek(mp3FilePointer,0,SEEK_END);
    uint32_t fileSize = ftell(mp3FilePointer);
    fseek(mp3FilePointer,0,SEEK_SET);
    unsigned char *dataBuffer = (unsigned char *)malloc(fileSize);
    if(!dataBuffer){
      printf("error");
      fclose(mp3FilePointer);
      return;
    }
    fread(dataBuffer,1,fileSize,mp3FilePointer);
    FILE *temp = fopen("temp.mp3","w");
    if(!temp){
      printf("error");
      fclose(temp);
      return;
    }
    // header
    fwrite(&ID3Tag->header,1,sizeof(ID3Tag->header),temp);

    //TXTFrames
    ID3v2TXTFrameType *TXTFrame;
    ListFramePtr_setFirstActive(&ID3Tag->TXTFrameList);
    for(int i = 0; i<5; i++){
      TXTFrame = (ID3v2TXTFrameType *)ListFramePtr_getActiveFramePtr(ID3Tag->TXTFrameList);
      fwrite(&TXTFrame->header,1, sizeof(TXTFrame->header),temp);
      fwrite(&TXTFrame->textEncoding,1, 1,temp);
      fwrite(TXTFrame->content.string,1, TxtStr_getStringLen(TXTFrame->content),temp);
      ListFramePtr_setNextActive(&ID3Tag->TXTFrameList);
    }

    //PRIVFrames
    ID3v2PRIVFrameType *PRIVFrame;
    ListFramePtr_setFirstActive(&ID3Tag->PRIVFrameList);
    while(ID3Tag->PRIVFrameList.active != NULL){
      PRIVFrame = (ID3v2PRIVFrameType *)ListFramePtr_getActiveFramePtr(ID3Tag->PRIVFrameList);
      fwrite(&PRIVFrame->header,1, sizeof(PRIVFrame->header),temp);
      fwrite(PRIVFrame->owner.string,1, TxtStr_getStringLen(PRIVFrame->owner),temp);
      fwrite(PRIVFrame->privateData.string,1, TxtStr_getStringLen(PRIVFrame->privateData),temp);
      ListFramePtr_setNextActive(&ID3Tag->PRIVFrameList);
    }

    for(int i = 0; i<2; i++){
      TXTFrame = (ID3v2TXTFrameType *)ListFramePtr_getActiveFramePtr(ID3Tag->TXTFrameList);
      fwrite(&TXTFrame->header,1, sizeof(TXTFrame->header),temp);
      fwrite(&TXTFrame->textEncoding,1, 1,temp);
      fwrite(TXTFrame->content.string,1, TxtStr_getStringLen(TXTFrame->content),temp);
      ListFramePtr_setNextActive(&ID3Tag->TXTFrameList);
    }
    
    //COMMFrames
    ID3v2COMMFrameType *COMMFrame;
    ListFramePtr_setFirstActive(&ID3Tag->COMMFrameList);
    while(ID3Tag->COMMFrameList.active != NULL){
      COMMFrame = (ID3v2COMMFrameType *) ListFramePtr_getActiveFramePtr(ID3Tag->COMMFrameList);
      fwrite(&COMMFrame->header,1, sizeof(COMMFrame->header),temp);
      fwrite(&COMMFrame->textEncoding,1, 1,temp);
      fwrite(&COMMFrame->language,1, 3,temp);
      fwrite(COMMFrame->contentDescript.string,1, TxtStr_getStringLen(COMMFrame->contentDescript),temp);
      fwrite(COMMFrame->actualText.string,1, TxtStr_getStringLen(COMMFrame->actualText),temp);
      ListFramePtr_setNextActive(&ID3Tag->COMMFrameList);
    }
    //TXTFrames
    // ID3v2TXTFrameType *TXTFrame;
    // ListFramePtr_setFirstActive(&ID3Tag->TXTFrameList);
    while(ID3Tag->TXTFrameList.active != NULL){
      TXTFrame = (ID3v2TXTFrameType *)ListFramePtr_getActiveFramePtr(ID3Tag->TXTFrameList);
      fwrite(&TXTFrame->header,1, sizeof(TXTFrame->header),temp);
      fwrite(&TXTFrame->textEncoding,1, 1,temp);
      fwrite(TXTFrame->content.string,1, TxtStr_getStringLen(TXTFrame->content),temp);
      ListFramePtr_setNextActive(&ID3Tag->TXTFrameList);
    }
    
    //POPM
    if(ID3Tag->POPM != NULL){
      fwrite(&ID3Tag->POPM->header,1, sizeof(ID3Tag->POPM->header),temp);
      fwrite(ID3Tag->POPM->userEmail.string,1, TxtStr_getStringLen(ID3Tag->POPM->userEmail),temp);
      fwrite(&ID3Tag->POPM->rating,1, 1,temp);
      fwrite(&ID3Tag->POPM->counter,1, 4,temp);
    }
    //APIC
    ID3v2APICFrameType *APICFrame;
    ListFramePtr_setFirstActive(&ID3Tag->APICFrameList);
    while(ID3Tag->APICFrameList.active != NULL){
      APICFrame = (ID3v2APICFrameType *)ListFramePtr_getActiveFramePtr(ID3Tag->APICFrameList);
      fwrite(&APICFrame->header,1, sizeof(APICFrame->header),temp);
      fwrite(&APICFrame->textEncoding,1, 1,temp);
      fwrite(APICFrame->mimeType.string,1, TxtStr_getStringLen(APICFrame->mimeType),temp);
      fwrite(&APICFrame->pictureType,1, 1,temp);
      fwrite(APICFrame->description.string,1, TxtStr_getStringLen(APICFrame->description),temp);
      fwrite(APICFrame->imageData,1,APICFrame->imageDataSize ,temp);
      ListFramePtr_setNextActive(&ID3Tag->APICFrameList);
    }
    //Padding
    char zero = 0;
    for (int i = 0; i < (int) ID3Tag->paddingSize; i++) {
        fwrite(&zero, 1, 1, temp);
    }

    // content
    fwrite(dataBuffer,1,fileSize,temp);
    fclose(temp);

    remove(file);
    rename("temp.mp3",file);
  }
}

void ELITEFOUR_FileManager_writteTagIntoFile(char *file, ID3TagType *ID3Tag){
  FileManager_removeTagFromFile(file);
  FILE *mp3FilePointer = fopen(file,"r");
  if(mp3FilePointer){
    fseek(mp3FilePointer,0,SEEK_END);
    uint32_t fileSize = ftell(mp3FilePointer);
    fseek(mp3FilePointer,0,SEEK_SET);
    unsigned char *dataBuffer = (unsigned char *)malloc(fileSize);
    if(!dataBuffer){
      printf("error");
      fclose(mp3FilePointer);
      return;
    }
    fread(dataBuffer,1,fileSize,mp3FilePointer);
    FILE *temp = fopen("temp.mp3","w");
    if(!temp){
      printf("error");
      fclose(temp);
      return;
    }
    // header
    fwrite(&ID3Tag->header,1,sizeof(ID3Tag->header),temp);

    //APIC
    ID3v2APICFrameType *APICFrame;
    ListFramePtr_setFirstActive(&ID3Tag->APICFrameList);
    while(ID3Tag->APICFrameList.active != NULL){
      APICFrame = (ID3v2APICFrameType *)ListFramePtr_getActiveFramePtr(ID3Tag->APICFrameList);
      fwrite(&APICFrame->header,1, sizeof(APICFrame->header),temp);
      fwrite(&APICFrame->textEncoding,1, 1,temp);
      fwrite(APICFrame->mimeType.string,1, TxtStr_getStringLen(APICFrame->mimeType),temp);
      fwrite(&APICFrame->pictureType,1, 1,temp);
      fwrite(APICFrame->description.string,1, TxtStr_getStringLen(APICFrame->description),temp);
      fwrite(APICFrame->imageData,1,APICFrame->imageDataSize ,temp);
      ListFramePtr_setNextActive(&ID3Tag->APICFrameList);
    }
    
    //TXTFrames
    ID3v2TXTFrameType *TXTFrame;
    ListFramePtr_setFirstActive(&ID3Tag->TXTFrameList);
    for(int i = 0; i<3; i++){
      TXTFrame = (ID3v2TXTFrameType *)ListFramePtr_getActiveFramePtr(ID3Tag->TXTFrameList);
      fwrite(&TXTFrame->header,1, sizeof(TXTFrame->header),temp);
      fwrite(&TXTFrame->textEncoding,1, 1,temp);
      fwrite(TXTFrame->content.string,1, TxtStr_getStringLen(TXTFrame->content),temp);
      ListFramePtr_setNextActive(&ID3Tag->TXTFrameList);
    }
     
    //PRIVFrames
    ID3v2PRIVFrameType *PRIVFrame;
    ListFramePtr_setFirstActive(&ID3Tag->PRIVFrameList);
    for(int i = 0; i<3; i++){
      PRIVFrame = (ID3v2PRIVFrameType *)ListFramePtr_getActiveFramePtr(ID3Tag->PRIVFrameList);
      fwrite(&PRIVFrame->header,1, sizeof(PRIVFrame->header),temp);
      fwrite(PRIVFrame->owner.string,1, TxtStr_getStringLen(PRIVFrame->owner),temp);
      fwrite(PRIVFrame->privateData.string,1, TxtStr_getStringLen(PRIVFrame->privateData),temp);
      ListFramePtr_setNextActive(&ID3Tag->PRIVFrameList);
      
    }

    //TXTFrames
    for(int i = 0; i<3; i++){
      TXTFrame = (ID3v2TXTFrameType *)ListFramePtr_getActiveFramePtr(ID3Tag->TXTFrameList);
      fwrite(&TXTFrame->header,1, sizeof(TXTFrame->header),temp);
      fwrite(&TXTFrame->textEncoding,1, 1,temp);
      fwrite(TXTFrame->content.string,1, TxtStr_getStringLen(TXTFrame->content),temp);
      ListFramePtr_setNextActive(&ID3Tag->TXTFrameList);
    }

    //MCDI
    fwrite(&ID3Tag->MCDI->header,1, sizeof(ID3Tag->MCDI->header),temp);
    fwrite(ID3Tag->MCDI->CDTOC.string,1, TxtStr_getStringLen(ID3Tag->MCDI->CDTOC),temp);

    //PRIVFrames
    for(int i = 0; i<3; i++){
      PRIVFrame = (ID3v2PRIVFrameType *)ListFramePtr_getActiveFramePtr(ID3Tag->PRIVFrameList);
      fwrite(&PRIVFrame->header,1, sizeof(PRIVFrame->header),temp);
      fwrite(PRIVFrame->owner.string,1, TxtStr_getStringLen(PRIVFrame->owner),temp);
      fwrite(PRIVFrame->privateData.string,1, TxtStr_getStringLen(PRIVFrame->privateData),temp);
      ListFramePtr_setNextActive(&ID3Tag->PRIVFrameList);
    }

    TXTFrame = (ID3v2TXTFrameType *)ListFramePtr_getActiveFramePtr(ID3Tag->TXTFrameList);
    fwrite(&TXTFrame->header,1, sizeof(TXTFrame->header),temp);
    fwrite(&TXTFrame->textEncoding,1, 1,temp);
    fwrite(TXTFrame->content.string,1, TxtStr_getStringLen(TXTFrame->content),temp);
    ListFramePtr_setNextActive(&ID3Tag->TXTFrameList);

    for(int i = 0; i<4; i++){
      PRIVFrame = (ID3v2PRIVFrameType *)ListFramePtr_getActiveFramePtr(ID3Tag->PRIVFrameList);
      fwrite(&PRIVFrame->header,1, sizeof(PRIVFrame->header),temp);
      fwrite(PRIVFrame->owner.string,1, TxtStr_getStringLen(PRIVFrame->owner),temp);
      fwrite(PRIVFrame->privateData.string,1, TxtStr_getStringLen(PRIVFrame->privateData),temp);
      ListFramePtr_setNextActive(&ID3Tag->PRIVFrameList);
    }
    TXTFrame = (ID3v2TXTFrameType *)ListFramePtr_getActiveFramePtr(ID3Tag->TXTFrameList);
    fwrite(&TXTFrame->header,1, sizeof(TXTFrame->header),temp);
    fwrite(&TXTFrame->textEncoding,1, 1,temp);
    fwrite(TXTFrame->content.string,1, TxtStr_getStringLen(TXTFrame->content),temp);
    ListFramePtr_setNextActive(&ID3Tag->TXTFrameList);
    TXTFrame = (ID3v2TXTFrameType *)ListFramePtr_getActiveFramePtr(ID3Tag->TXTFrameList);
    fwrite(&TXTFrame->header,1, sizeof(TXTFrame->header),temp);
    fwrite(&TXTFrame->textEncoding,1, 1,temp);
    fwrite(TXTFrame->content.string,1, TxtStr_getStringLen(TXTFrame->content),temp);

    //Padding
    char zero = 0;
    for (int i = 0; i < (int) ID3Tag->paddingSize; i++) {
        fwrite(&zero, 1, 1, temp);
    }

    // content
    fwrite(dataBuffer,1,fileSize,temp);
    fclose(temp);

    remove(file);
    rename("temp.mp3",file);
  }
}
