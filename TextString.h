#ifndef TEXTSTRING_H
#define TEXTSTRING_H

#include <stdbool.h>
typedef struct {
  char *string;
  bool hasEndOfString;
  int size; //including end of string if it has
} TextStringType;


void TxtStr_storeTextString(TextStringType *, char *, bool);

int TxtStr_getStringLen(TextStringType);

void TxtStr_freeTextString(TextStringType *);

#endif //TEXTSTRING_H