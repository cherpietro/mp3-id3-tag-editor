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

void initCOMMFrameList(COMMFrameList *);

void insertLastCOMMFrameList(COMMFrameList *, ID3v2COMMFrameType);

void setFirstActiveCOMMFrameList(COMMFrameList *);

void setNextActiveCOMMFrameList(COMMFrameList *);

void deleteActiveCOMMFrameList(COMMFrameList *);

ID3v2COMMFrameType getCOMMFrameListActive(COMMFrameList);

bool isEmptyCOMMFrameList(COMMFrameList);

void freeCOMMFrameList(COMMFrameList *);
#endif // COMM_FRAME_LIST_H
