#include "TextString.h"
#include <string.h>
#include <stdlib.h>

void TxtStr_storeTextString(TextStringType *txtStr, char *str, size_t sizeToSave){

    txtStr->string =  (char *)malloc(sizeToSave);;
    memcpy(txtStr->string,str,sizeToSave);
    txtStr->size = sizeToSave;
    txtStr->hasEndOfString = txtStr->string[txtStr->size-1] == '\0';
}

size_t TxtStr_getStringLen(TextStringType txtStr){
  return txtStr.size;
}

void TxtStr_freeTextString(TextStringType *txtStr){
  if(txtStr->string != NULL){
    free(txtStr->string);
    txtStr->string = NULL;
    txtStr->size = 0;
    txtStr->hasEndOfString = false;
  }
}