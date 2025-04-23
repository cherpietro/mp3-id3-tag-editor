#ifndef COMM_FRAME_LIST_H
#define COMM_FRAME_LIST_H
#include <stdbool.h>
#include "ID3v2Frames.h"

typedef struct CommFrLstElement{
  struct CommFrLstElement *prev;
  ID3v2COMMFrameType frame;
  struct CommFrLstElement *next;
}COMMFrameListElement;

typedef struct{
  COMMFrameListElement *first;
  COMMFrameListElement *last;
  COMMFrameListElement *active;
}COMMFrameList;

void ListCOMM_init(COMMFrameList *);

void ListCOMM_insertLast(COMMFrameList *, ID3v2COMMFrameType);

void ListCOMM_setFirstActive(COMMFrameList *);

void ListCOMM_setNextActive(COMMFrameList *);

void ListCOMM_deleteActive(COMMFrameList *);

ID3v2COMMFrameType ListCOMM_getActive(COMMFrameList);

bool ListCOMM_isEmpty(COMMFrameList);

void ListCOMM_freeList(COMMFrameList *);
#endif // COMM_FRAME_LIST_H
