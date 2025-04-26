#include "TextString.h"
#include "string.h"
void TxtStr_storeTextString(TextStringType *txtStr, char *str, int sizeToSave){

    txtStr->string =  (char *)malloc(sizeToSave);;
    strncpy(txtStr->string,txtStr,sizeToSave);
    txtStr->size = sizeToSave;
    txtStr->hasEndOfString = txtStr->string[txtStr->size-1] == '\0';
}

int TxtStr_getStringLen(TextStringType txtStr){
  return txtStr.size;
}

void TxtStr_freeTextString(TextStringType *txtStr){
  if(txtStr->string != NULL){
    free(txtStr);
  }
}