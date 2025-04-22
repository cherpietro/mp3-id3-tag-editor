#include "TextFrameList.h"
#include <string.h>
#include <stdlib.h>

void ListTXTF_init(TextFrameList *list) {
    list->first = NULL;
    list->last = NULL;
    list->active = NULL;
}

void ListTXTF_insertLast(TextFrameList *list, ID3v2TextFrameType frame) {
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

void ListTXTF_setFirstActive(TextFrameList *list) {
    list->active = list->first;
}

void ListTXTF_setNextActive(TextFrameList *list) {
    if (list->active != NULL)
        list->active = list->active->next;
}

void ListTXTF_deleteActive(TextFrameList *list) {
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

bool ListTXTF_isEmpty(TextFrameList list) {
    return (list.first == NULL && list.last == NULL);
}

void ListTXTF_freeList(TextFrameList *list) {
    ListTXTF_setFirstActive(list);
    while (!ListTXTF_isEmpty(*list)) {
        ListTXTF_deleteActive(list);
    }
}

ID3v2TextFrameType ListTXTF_getActive(TextFrameList list){
  return list.active->frame;
}

