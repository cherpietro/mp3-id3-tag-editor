#ifndef TEXT_FRAME_LIST_H
#define TEXT_FRAME_LIST_H
#include <stdbool.h>
#include "ID3v2Frames.h"

typedef struct TxtFrLstElement{
  struct TxtFrLstElement *prev;
  ID3v2TXTFrameType frame;
  struct TxtFrLstElement *next;
}TXTFrameListElement;

typedef struct{
  TXTFrameListElement *first;
  TXTFrameListElement *last;
  TXTFrameListElement *active;
}TXTFrameList;

void ListTXTF_init(TXTFrameList *);

void ListTXTF_insertLast(TXTFrameList *, ID3v2TXTFrameType);

void ListTXTF_setFirstActive(TXTFrameList *);

void ListTXTF_setNextActive(TXTFrameList *);

void ListTXTF_deleteActive(TXTFrameList *);

ID3v2TXTFrameType ListTXTF_getActive(TXTFrameList);

bool ListTXTF_isEmpty(TXTFrameList);

void ListTXTF_freeList(TXTFrameList *);
#endif // TEXT_FRAME_LIST_H
