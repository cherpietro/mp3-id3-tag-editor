#include "TextFrameList.h"
#include "TextString.h"
#include <string.h>
#include <stdlib.h>

void ListTXTF_init(TextFrameList *list) {
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

void ListTXTF_insertLast(TextFrameList *list, ID3v2TextFrameType frame) {
    TextFrameListElement *newElemPtr = (TextFrameListElement *)malloc(sizeof(TextFrameListElement));
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

bool ListTXTF_isEmpty(TextFrameList list) {
    return (list.first == NULL && list.last == NULL);
}

void ListTXTF_freeList(TextFrameList *list) {
    ListTXTF_setFirstActive(list);
    while (!ListTXTF_isEmpty(*list)) {
        ListTXTF_deleteActive(list);
    }
}

ID3v2TextFrameType ListTXTF_getActive(TextFrameList list) {
    return list.active->frame;
}
