#include "COMMFrameList.h"
#include <string.h>
#include <stdlib.h>

void ListCOMM_init(COMMFrameList *list) {
    list->first = NULL;
    list->last = NULL;
    list->active = NULL;
}

void ListCOMM_insertLast(COMMFrameList *list, ID3v2COMMFrameType frame) {
    COMMFrameListElement *newElemPtr = (COMMFrameListElement *)malloc(sizeof(COMMFrameListElement));
    if (!newElemPtr) return;

    newElemPtr->frame = frame;
    newElemPtr->frame.actualText = frame.actualText ? strdup(frame.actualText) : NULL;

    if (frame.actualText && !newElemPtr->frame.actualText) {
        free(newElemPtr);
        return;
    }

    newElemPtr->frame.contentDescript = frame.contentDescript ? strdup(frame.contentDescript) : NULL;

    if (frame.contentDescript && !newElemPtr->frame.contentDescript) {
        free(newElemPtr);
        return;
    }

    newElemPtr->next = NULL;
    newElemPtr->prev = list->last;

    if (list->last != NULL) {
        list->last->next = newElemPtr;
    } else {
        // La lista estaba vacía
        list->first = newElemPtr;
    }

    list->last = newElemPtr;
}

void ListCOMM_setFirstActive(COMMFrameList *list) {
    list->active = list->first;
}

void ListCOMM_setNextActive(COMMFrameList *list) {
    if (list->active != NULL)
        list->active = list->active->next;
}

void ListCOMM_deleteActive(COMMFrameList *list) {
    if (list->active == NULL) return;

    COMMFrameListElement *toDelete = list->active;

    if (toDelete == list->first && toDelete == list->last) {
        // Único elemento
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

    free(toDelete->frame.contentDescript);
    free(toDelete->frame.actualText);
    free(toDelete);
}

bool ListCOMM_isEmpty(COMMFrameList list) {
    return (list.first == NULL && list.last == NULL);
}

void ListCOMM_freeList(COMMFrameList *list) {
    ListCOMM_setFirstActive(list);
    while (!ListCOMM_isEmpty(*list)) {
        ListCOMM_deleteActive(list);
    }
}

ID3v2COMMFrameType ListCOMM_getActive(COMMFrameList list){
  return list.active->frame;
}

