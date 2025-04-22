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

void ListTXTF_init(TextFrameList *);

void ListTXTF_insertLast(TextFrameList *, ID3v2TextFrameType);

void ListTXTF_setFirstActive(TextFrameList *);

void ListTXTF_setNextActive(TextFrameList *);

void ListTXTF_deleteActive(TextFrameList *);

ID3v2TextFrameType ListTXTF_getActive(TextFrameList);

bool ListTXTF_isEmpty(TextFrameList);

void ListTXTF_freeList(TextFrameList *);
#endif // TEXT_FRAME_LIST_H
