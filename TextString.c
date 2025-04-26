#include "TextString.h"
#include "string.h"
void TxtStr_storeTextString(TextStringType *txtStr, char *str, bool hasEndOfString){
  int lenOfStrin = strlen(str);
  if(hasEndOfString){
    txtStr->string = (char*) malloc(lenOfStrin+1);
    strncpy(txtStr->string,txtStr,lenOfStrin);
    txtStr->string[lenOfStrin] = '\0';
    txtStr->size = lenOfStrin + 1;
  }
  else{
    txtStr->string = (char*) malloc(lenOfStrin);
    strncpy(txtStr->string,txtStr,lenOfStrin);
    txtStr->size = lenOfStrin;
  }
  txtStr->hasEndOfString = hasEndOfString;
}

int TxtStr_getStringLen(TextStringType txtStr){
  return txtStr.size;
}

void TxtStr_freeTextString(TextStringType *txtStr){
  if(txtStr->string != NULL){
    free(txtStr);
  }
}