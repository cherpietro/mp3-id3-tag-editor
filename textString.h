#ifndef TEXTSTRING_H
#define TEXTSTRING_H

#include <stdbool.h>
typedef struct {
  char *string;
  bool hasEndOfString;
  int size; //including end of string if it has
} TextStringType;


void storeTextString(TextStringType *, char *, bool);

int getStringLen(TextStringType);

void freeTextString(TextStringType *);

#endif //TEXTSTRING_H