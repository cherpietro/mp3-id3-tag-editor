#ifndef LIST_FRAME_PTR_H
#define LIST_FRAME_PTR_H
#include <stdbool.h>
#include "ID3v2Frames.h"

typedef struct LstFrPtr{
  struct LstFrPtr *prev;
  void *frame;
  struct LstFrPtr *next;
}ListFramePtrElement;

typedef struct{
  ListFramePtrElement *first;
  ListFramePtrElement *last;
  ListFramePtrElement *active;
}ListFramePtr;

void ListFramePtr_init(ListFramePtr *);

void ListFramePtr_setFirstActive(ListFramePtr *);

void ListFramePtr_setNextActive(ListFramePtr *);

void ListFramePtr_insertLast(ListFramePtr *, void *);

void ListFramePtr_deleteActive(ListFramePtr *);

void *ListFramePtr_getActiveFramePtr(ListFramePtr);

bool ListFramePtr_isEmpty(ListFramePtr);

// void ListFramePtr_freeList(ListFramePtr *);
#endif // LIST_FRAME_PTR_H
