
#include "PRIVFrameList.h"
#include <string.h>
#include <stdlib.h>

void ListPRIV_init(PRIVFrameList *list) {
    list->first = NULL;
    list->last = NULL;
    list->active = NULL;
}

// Función auxiliar para copiar un TextStringType
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

// Función auxiliar para liberar un TextStringType
static void freeTextString(TextStringType *txt) {
    if (txt->string) {
        free(txt->string);
        txt->string = NULL;
    }
    txt->size = 0;
    txt->hasEndOfString = false;
}

void ListPRIV_insertLast(PRIVFrameList *list, ID3v2PRIVFrameType frame) {
    PRIVFrameListElement *newElemPtr = (PRIVFrameListElement *)malloc(sizeof(PRIVFrameListElement));
    if (!newElemPtr) return;

    newElemPtr->frame = frame;
    newElemPtr->frame.owner = copyTextString(&frame.owner);

    if (frame.owner.string && !newElemPtr->frame.owner.string) {
        free(newElemPtr);
        return;
    }

    newElemPtr->frame.privateData = copyTextString(&frame.privateData);

    if (frame.privateData.string && !newElemPtr->frame.privateData.string) {
        freeTextString(&newElemPtr->frame.privateData);
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

void ListPRIV_setFirstActive(PRIVFrameList *list) {
    list->active = list->first;
}

void ListPRIV_setNextActive(PRIVFrameList *list) {
    if (list->active != NULL)
        list->active = list->active->next;
}

void ListPRIV_deleteActive(PRIVFrameList *list) {
    if (list->active == NULL) return;

    PRIVFrameListElement *toDelete = list->active;

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

    freeTextString(&toDelete->frame.privateData);
    freeTextString(&toDelete->frame.owner);
    free(toDelete);
}

bool ListPRIV_isEmpty(PRIVFrameList list) {
    return (list.first == NULL && list.last == NULL);
}

void ListPRIV_freeList(PRIVFrameList *list) {
    ListPRIV_setFirstActive(list);
    while (!ListPRIV_isEmpty(*list)) {
        ListPRIV_deleteActive(list);
    }
}

ID3v2PRIVFrameType ListPRIV_getActive(PRIVFrameList list) {
    return list.active->frame;
}
