#include "ListFramePtr.h"
#include "TextString.h"
#include <string.h>
#include <stdlib.h>

void ListFramePtr_init(ListFramePtr *list) {
    list->first = NULL;
    list->last = NULL;
    list->active = NULL;
}

void ListFramePtr_setFirstActive(ListFramePtr *list) {
  list->active = list->first;
}

void ListFramePtr_setNextActive(ListFramePtr *list) {
  if (list->active != NULL)
      list->active = list->active->next;
}

void ListFramePtr_insertLast(ListFramePtr *list, void *frame) {
    ListFramePtrElement *newElemPtr = (ListFramePtrElement *)malloc(sizeof(ListFramePtrElement));
    newElemPtr->frame = frame;
    newElemPtr->next = NULL;
    newElemPtr->prev = list->last;

    if (list->last != NULL) {
        list->last->next = newElemPtr;
    } else {
        list->first = newElemPtr;
    }

    list->last = newElemPtr;
}

void ListFramePtr_deleteActive(ListFramePtr *list) {
    //ENSURE FRAME IS REMOVED BEFORE
    if (list->active == NULL) return;
    ListFramePtrElement *toDelete = list->active;
    if (toDelete == list->first && toDelete == list->last) {
        list->first = NULL;
        list->last = NULL;
        list->active = NULL;
    } else if (toDelete == list->first) {
        list->first = toDelete->next;
        list->first->prev = NULL;
        list->active = list->first;
    } else if (toDelete == list->last) {
        list->last = toDelete->prev;
        list->last->next = NULL;
        list->active = NULL;
    } else {
        toDelete->prev->next = toDelete->next;
        toDelete->next->prev = toDelete->prev;
        list->active = toDelete->next;
    }

    free(toDelete);
    toDelete = NULL;
}

bool ListFramePtr_isEmpty(ListFramePtr list) {
    return (list.first == NULL && list.last == NULL);
}

// void ListFramePtr_freeList(ListFramePtr *list) {
//     ListFramePtr_setFirstActive(list);
//     while (!ListFramePtr_isEmpty(*list)) {
//         ListFramePtr_deleteActive(list);
//     }
// }

void *ListFramePtr_getActiveFramePtr(ListFramePtr list) {
    if(list.active != NULL) return list.active->frame;
    return NULL;
}
