#include "TXTFrameList.h"
#include "TextString.h"
#include <string.h>
#include <stdlib.h>

void ListTXTF_init(TXTFrameList *list) {
    list->first = NULL;
    list->last = NULL;
    list->active = NULL;
}

static TextStringType copyTextString(const TextStringType *src) {
    TextStringType dest;
    dest.hasEndOfString = src->hasEndOfString;
    dest.size = src->size;
    if (src->string && src->size > 0) {
        dest.string = (char *)malloc(src->size);
        if (dest.string) {
            memcpy(dest.string, src->string, src->size);
        }
    } else {
        dest.string = NULL;
    }
    return dest;
}

void ListTXTF_insertLast(TXTFrameList *list, ID3v2TXTFrameType frame) {
    TXTFrameListElement *newElemPtr = (TXTFrameListElement *)malloc(sizeof(TXTFrameListElement));
    if (!newElemPtr) return;

    newElemPtr->frame = frame;
    newElemPtr->frame.content = copyTextString(&frame.content);

    if (frame.content.string && !newElemPtr->frame.content.string) {
        free(newElemPtr);
        return;
    }

    newElemPtr->next = NULL;
    newElemPtr->prev = list->last;

    if (list->last != NULL) {
        list->last->next = newElemPtr;
    } else {
        list->first = newElemPtr;
    }

    list->last = newElemPtr;
}

void ListTXTF_setFirstActive(TXTFrameList *list) {
    list->active = list->first;
}

void ListTXTF_setNextActive(TXTFrameList *list) {
    if (list->active != NULL)
        list->active = list->active->next;
}

void ListTXTF_deleteActive(TXTFrameList *list) {
    if (list->active == NULL) return;

    TXTFrameListElement *toDelete = list->active;

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
    TxtStr_freeTextString(&toDelete->frame.content);
    free(toDelete);
}

bool ListTXTF_isEmpty(TXTFrameList list) {
    return (list.first == NULL && list.last == NULL);
}

void ListTXTF_freeList(TXTFrameList *list) {
    ListTXTF_setFirstActive(list);
    while (!ListTXTF_isEmpty(*list)) {
        ListTXTF_deleteActive(list);
    }
}

ID3v2TXTFrameType ListTXTF_getActive(TXTFrameList list) {
    return list.active->frame;
}

TXTFrameListElement * ListTXTF_SearchFrame(TXTFrameList *list, bool firstIteration, char *frameID){
    if(firstIteration) ListTXTF_setFirstActive(list);
    else ListTXTF_setNextActive(list);
    ID3v2TXTFrameType searchedFrame = ListTXTF_getActive(*list);
    while(list->active != NULL && strncmp(frameID,searchedFrame.header.frameId,4) != 0){
        ListTXTF_setNextActive(list);
    }
    return list->active;
}