#include "COMMFrameList.h"
#include <string.h>
#include <stdlib.h>

void initCOMMFrameList(COMMFrameList *list) {
    list->first = NULL;
    list->last = NULL;
    list->active = NULL;
}

void insertLastCOMMFrameList(COMMFrameList *list, ID3v2COMMFrameType frame) {
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

void setFirstActiveCOMMFrameList(COMMFrameList *list) {
    list->active = list->first;
}

void setNextActiveCOMMFrameList(COMMFrameList *list) {
    if (list->active != NULL)
        list->active = list->active->next;
}

void deleteActiveCOMMFrameList(COMMFrameList *list) {
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

bool isEmptyCOMMFrameList(COMMFrameList list) {
    return (list.first == NULL && list.last == NULL);
}

void freeCOMMFrameList(COMMFrameList *list) {
    setFirstActiveCOMMFrameList(list);
    while (!isEmptyCOMMFrameList(*list)) {
        deleteActiveCOMMFrameList(list);
    }
}

ID3v2COMMFrameType getCOMMFrameListActive(COMMFrameList list){
  return list.active->frame;
}

