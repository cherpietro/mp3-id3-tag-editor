#ifndef PRIV_FRAME_LIST_H
#define PRIV_FRAME_LIST_H
#include <stdbool.h>
#include "ID3v2Frames.h"

typedef struct PrivFrLstElement{
  struct PrivFrLstElement *prev;
  ID3v2PRIVFrameType frame;
  struct PrivFrLstElement *next;
}PRIVFrameListElement;

typedef struct{
  PRIVFrameListElement *first;
  PRIVFrameListElement *last;
  PRIVFrameListElement *active;
}PRIVFrameList;

void ListPRIV_init(PRIVFrameList *);

void ListPRIV_insertLast(PRIVFrameList *, ID3v2PRIVFrameType);

void ListPRIV_setFirstActive(PRIVFrameList *);

void ListPRIV_setNextActive(PRIVFrameList *);

void ListPRIV_deleteActive(PRIVFrameList *);

ID3v2PRIVFrameType ListPRIV_getActive(PRIVFrameList);

bool ListPRIV_isEmpty(PRIVFrameList);

void ListPRIV_freeList(PRIVFrameList *);
#endif // PRIV_FRAME_LIST_H

