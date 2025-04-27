#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "ID3v2.h"

void testOverture();

void testTimberHearth();

void testJokin3();

void testEliteFour();

int main(){
  testEliteFour();
  testOverture();
  testTimberHearth();
  testJokin3();
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

void testEliteFour(){
  ID3TagType ID3Tag;
  char *file = "./files/auxFiles/EliteFour.mp3";  
  ID3v2_init(&ID3Tag);
  ID3v2_storeTagInStruct(file,&ID3Tag);
  ELITEFOUR_ID3v2_writteTagIntoFile(file,&ID3Tag);
  ID3v2_free(&ID3Tag);
  
  printf("Difference between files: \n");
  system("cmp -l ./files/EliteFour.mp3 ./files/auxFiles/EliteFour.mp3 | wc -l");
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

void testJokin3(){
  ID3TagType ID3Tag;
  char *file = "./files/auxFiles/jokin3.mp3";
  ID3v2_init(&ID3Tag);
  ID3v2_storeTagInStruct(file,&ID3Tag);
  JOKIN3_ID3v2_writteTagIntoFile(file,&ID3Tag);
  ID3v2_free(&ID3Tag);
  
  printf("Difference between files: \n");
  system("cmp -l ./files/jokin3.mp3 ./files/auxFiles/jokin3.mp3 | wc -l");
}


void TIMBERHEARTH_ID3v2_writteTagIntoFile(char *file, ID3TagType *ID3Tag){
  ID3v2_removeTagFromFile(file);

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
    ID3v2TextFrameType TXTFrame;
    ListTXTF_setFirstActive(&ID3Tag->textFrameList);
    TXTFrame = ListTXTF_getActive(ID3Tag->textFrameList);
    fwrite(&TXTFrame.header,1, sizeof(TXTFrame.header),temp);
    fwrite(&TXTFrame.textEncoding,1, 1,temp);
    fwrite(TXTFrame.content.string,1, TXTFrame.content.size,temp);
    ListTXTF_setNextActive(&ID3Tag->textFrameList);
    /**/
    //COMMFrames
    ID3v2COMMFrameType COMMFrame;
    ListCOMM_setFirstActive(&ID3Tag->COMMFrameList);
    while(ID3Tag->COMMFrameList.active != NULL){
      COMMFrame = ListCOMM_getActive(ID3Tag->COMMFrameList);
      fwrite(&COMMFrame.header,1, sizeof(COMMFrame.header),temp);
      fwrite(&COMMFrame.textEncoding,1, 1,temp);
      fwrite(&COMMFrame.language,1, 3,temp);
      fwrite(COMMFrame.contentDescript.string,1, TxtStr_getStringLen(COMMFrame.contentDescript),temp);
      fwrite(COMMFrame.actualText.string,1, TxtStr_getStringLen(COMMFrame.actualText),temp);
      ListCOMM_setNextActive(&ID3Tag->COMMFrameList);
    }
    while(ID3Tag->textFrameList.active != NULL){
      TXTFrame = ListTXTF_getActive(ID3Tag->textFrameList);
      fwrite(&TXTFrame.header,1, sizeof(TXTFrame.header),temp);
      fwrite(&TXTFrame.textEncoding,1, 1,temp);
      fwrite(TXTFrame.content.string,1, TxtStr_getStringLen(TXTFrame.content),temp);
      ListTXTF_setNextActive(&ID3Tag->textFrameList);
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

void JOKIN3_ID3v2_writteTagIntoFile(char *file, ID3TagType *ID3Tag){
  ID3v2_removeTagFromFile(file);
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
     ID3v2TextFrameType TXTFrame;
     ListTXTF_setFirstActive(&ID3Tag->textFrameList);
     while(ID3Tag->textFrameList.active != NULL){
       TXTFrame = ListTXTF_getActive(ID3Tag->textFrameList);
       fwrite(&TXTFrame.header,1, sizeof(TXTFrame.header),temp);
       fwrite(&TXTFrame.textEncoding,1, 1,temp);
       fwrite(TXTFrame.content.string,1, TxtStr_getStringLen(TXTFrame.content),temp);
       ListTXTF_setNextActive(&ID3Tag->textFrameList);
     }
     
    //APIC
    if(ID3Tag->APIC != NULL){
      fwrite(&ID3Tag->APIC->header,1, sizeof(ID3Tag->APIC->header),temp);
      fwrite(&ID3Tag->APIC->textEncoding,1, 1,temp);
      fwrite(ID3Tag->APIC->mimeType.string,1, TxtStr_getStringLen(ID3Tag->APIC->mimeType),temp);
      fwrite(&ID3Tag->APIC->pictureType,1, 1,temp);
      fwrite(ID3Tag->APIC->description.string,1, TxtStr_getStringLen(ID3Tag->APIC->description),temp);
      fwrite(ID3Tag->APIC->imageData,1,ID3Tag->APIC->imageDataSize ,temp);
    }

    //COMMFrames
    ID3v2COMMFrameType COMMFrame;
    ListCOMM_setFirstActive(&ID3Tag->COMMFrameList);
    while(ID3Tag->COMMFrameList.active != NULL){
      COMMFrame = ListCOMM_getActive(ID3Tag->COMMFrameList);
      fwrite(&COMMFrame.header,1, sizeof(COMMFrame.header),temp);
      fwrite(&COMMFrame.textEncoding,1, 1,temp);
      fwrite(&COMMFrame.language,1, 3,temp);
      fwrite(COMMFrame.contentDescript.string,1, TxtStr_getStringLen(COMMFrame.contentDescript),temp);
      fwrite(COMMFrame.actualText.string,1, TxtStr_getStringLen(COMMFrame.actualText),temp);
      ListCOMM_setNextActive(&ID3Tag->COMMFrameList);
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

void ELITEFOUR_ID3v2_writteTagIntoFile(char *file, ID3TagType *ID3Tag){
  ID3v2_removeTagFromFile(file);
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
    if(ID3Tag->APIC != NULL){
      fwrite(&ID3Tag->APIC->header,1, sizeof(ID3Tag->APIC->header),temp);
      fwrite(&ID3Tag->APIC->textEncoding,1, 1,temp);
      fwrite(ID3Tag->APIC->mimeType.string,1, TxtStr_getStringLen(ID3Tag->APIC->mimeType),temp);
      fwrite(&ID3Tag->APIC->pictureType,1, 1,temp);
      fwrite(ID3Tag->APIC->description.string,1, TxtStr_getStringLen(ID3Tag->APIC->description),temp);
      fwrite(ID3Tag->APIC->imageData,1,ID3Tag->APIC->imageDataSize ,temp);
    }

    //TXTFrames
    ID3v2TextFrameType TXTFrame;
    ListTXTF_setFirstActive(&ID3Tag->textFrameList);
    for(int i = 0; i<3; i++){
      TXTFrame = ListTXTF_getActive(ID3Tag->textFrameList);
      fwrite(&TXTFrame.header,1, sizeof(TXTFrame.header),temp);
      fwrite(&TXTFrame.textEncoding,1, 1,temp);
      fwrite(TXTFrame.content.string,1, TxtStr_getStringLen(TXTFrame.content),temp);
      ListTXTF_setNextActive(&ID3Tag->textFrameList);
    }
     
    //PRIVFrames
    ID3v2PRIVFrameType PRIVFrame;
    ListPRIV_setFirstActive(&ID3Tag->PRIVFrameList);
    for(int i = 0; i<3; i++){
      PRIVFrame = ListPRIV_getActive(ID3Tag->PRIVFrameList);
      fwrite(&PRIVFrame.header,1, sizeof(PRIVFrame.header),temp);
      fwrite(PRIVFrame.owner.string,1, TxtStr_getStringLen(PRIVFrame.owner),temp);
      fwrite(PRIVFrame.privateData.string,1, TxtStr_getStringLen(PRIVFrame.privateData),temp);
      ListPRIV_setNextActive(&ID3Tag->PRIVFrameList);
    }

    //TXTFrames
    for(int i = 0; i<3; i++){
      TXTFrame = ListTXTF_getActive(ID3Tag->textFrameList);
      fwrite(&TXTFrame.header,1, sizeof(TXTFrame.header),temp);
      fwrite(&TXTFrame.textEncoding,1, 1,temp);
      fwrite(TXTFrame.content.string,1, TxtStr_getStringLen(TXTFrame.content),temp);
      ListTXTF_setNextActive(&ID3Tag->textFrameList);
    }

    //MCDI
    fwrite(&ID3Tag->MCDI->header,1, sizeof(ID3Tag->MCDI->header),temp);
    fwrite(ID3Tag->MCDI->CDTOC.string,1, TxtStr_getStringLen(ID3Tag->MCDI->CDTOC),temp);

    //PRIVFrames
    for(int i = 0; i<3; i++){
      PRIVFrame = ListPRIV_getActive(ID3Tag->PRIVFrameList);
      fwrite(&PRIVFrame.header,1, sizeof(PRIVFrame.header),temp);
      fwrite(PRIVFrame.owner.string,1, TxtStr_getStringLen(PRIVFrame.owner),temp);
      fwrite(PRIVFrame.privateData.string,1, TxtStr_getStringLen(PRIVFrame.privateData),temp);
      ListPRIV_setNextActive(&ID3Tag->PRIVFrameList);
    }

    TXTFrame = ListTXTF_getActive(ID3Tag->textFrameList);
    fwrite(&TXTFrame.header,1, sizeof(TXTFrame.header),temp);
    fwrite(&TXTFrame.textEncoding,1, 1,temp);
    fwrite(TXTFrame.content.string,1, TxtStr_getStringLen(TXTFrame.content),temp);
    ListTXTF_setNextActive(&ID3Tag->textFrameList);

    for(int i = 0; i<4; i++){
      PRIVFrame = ListPRIV_getActive(ID3Tag->PRIVFrameList);
      fwrite(&PRIVFrame.header,1, sizeof(PRIVFrame.header),temp);
      fwrite(PRIVFrame.owner.string,1, TxtStr_getStringLen(PRIVFrame.owner),temp);
      fwrite(PRIVFrame.privateData.string,1, TxtStr_getStringLen(PRIVFrame.privateData),temp);
      ListPRIV_setNextActive(&ID3Tag->PRIVFrameList);
    }
    TXTFrame = ListTXTF_getActive(ID3Tag->textFrameList);
    fwrite(&TXTFrame.header,1, sizeof(TXTFrame.header),temp);
    fwrite(&TXTFrame.textEncoding,1, 1,temp);
    fwrite(TXTFrame.content.string,1, TxtStr_getStringLen(TXTFrame.content),temp);
    ListTXTF_setNextActive(&ID3Tag->textFrameList);
    TXTFrame = ListTXTF_getActive(ID3Tag->textFrameList);
    fwrite(&TXTFrame.header,1, sizeof(TXTFrame.header),temp);
    fwrite(&TXTFrame.textEncoding,1, 1,temp);
    fwrite(TXTFrame.content.string,1, TxtStr_getStringLen(TXTFrame.content),temp);

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
