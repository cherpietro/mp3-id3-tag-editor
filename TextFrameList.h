#ifndef TEXT_FRAME_LIST_H
#define TEXT_FRAME_LIST_H
#include <stdbool.h>
#include "ID3v2Frames.h"

typedef struct TxtFrLstElement{
  struct TxtFrLstElement *prev;
  ID3v2TextFrameType frame;
  struct TxtFrLstElement *next;
}TextFrameListElement;

typedef struct{
  TextFrameListElement *first;
  TextFrameListElement *last;
  TextFrameListElement *active;
}TextFrameList;

void initTextFrameList(TextFrameList *);

void insertLastTextFrameList(TextFrameList *, ID3v2TextFrameType);

void setFirstActiveTextFrameList(TextFrameList *);

void setNextActiveTextFrameList(TextFrameList *);

void deleteActiveTextFrameList(TextFrameList *);

ID3v2TextFrameType getTextFrameListActive(TextFrameList);

bool isEmptyTextFrameList(TextFrameList);

void freeTextFrameList(TextFrameList *);
#endif // TEXT_FRAME_LIST_H
