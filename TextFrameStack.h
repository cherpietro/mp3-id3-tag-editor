#ifndef TEXT_FRAME_STACK_H
#define TEXT_FRAME_STACK_H
#include <stdbool.h>
#include "ID3v2Frames.h"

typedef struct TxtFrStckElement{
  ID3v2TextFrameType frame;
  struct TxtFrStckElement *next;
}TextFrameStackElement;

typedef struct{
  TextFrameStackElement *top;
}TextFrameStack;

void initTextFrameStack(TextFrameStack *);

void pushTextFrameStack(TextFrameStack *, ID3v2TextFrameType);

void popTextFrameStack(TextFrameStack *);

ID3v2TextFrameType topTextFrameStack(TextFrameStack);

bool isEmptyTextFrameStack(TextFrameStack *);

void freeTextFrameStack(TextFrameStack *);
#endif // TEXT_FRAME_STACK_H
