#include "TextFrameList.h"
#include <string.h>
#include <stdlib.h>

void initTextFrameList(TextFrameList *list) {
    list->first = NULL;
    list->last = NULL;
    list->active = NULL;
}

void insertLastTextFrameList(TextFrameList *list, ID3v2TextFrameType frame) {
    TextFrameListElement *newElemPtr = (TextFrameListElement *)malloc(sizeof(TextFrameListElement));
    if (!newElemPtr) return;

    newElemPtr->frame = frame;
    newElemPtr->frame.content = frame.content ? strdup(frame.content) : NULL;

    if (frame.content && !newElemPtr->frame.content) {
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

void setFirstActiveTextFrameList(TextFrameList *list) {
    list->active = list->first;
}

void setNextActiveTextFrameList(TextFrameList *list) {
    if (list->active != NULL)
        list->active = list->active->next;
}

void deleteActiveTextFrameList(TextFrameList *list) {
    if (list->active == NULL) return;

    TextFrameListElement *toDelete = list->active;

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

    free(toDelete->frame.content);
    free(toDelete);
}

bool isEmptyTextFrameList(TextFrameList list) {
    return (list.first == NULL && list.last == NULL);
}

void freeTextFrameList(TextFrameList *list) {
    setFirstActiveTextFrameList(list);
    while (!isEmptyTextFrameList(*list)) {
        deleteActiveTextFrameList(list);
    }
}

ID3v2TextFrameType getTextFrameListActive(TextFrameList list){
  return list.active->frame;
}

