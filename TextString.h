#ifndef TEXTSTRING_H
#define TEXTSTRING_H

#include <stdio.h>
#include <stdbool.h>
typedef struct {
  char *string;
  bool hasEndOfString;
  size_t size; //including end of string if it has
} TextStringType;


void TxtStr_storeTextString(TextStringType *, char *, size_t);

size_t TxtStr_getStringLen(TextStringType);

void TxtStr_freeTextString(TextStringType *);

#endif //TEXTSTRING_H