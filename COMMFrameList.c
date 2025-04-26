#include "COMMFrameList.h"
#include <string.h>
#include <stdlib.h>

void ListCOMM_init(COMMFrameList *list) {
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

void ListCOMM_insertLast(COMMFrameList *list, ID3v2COMMFrameType frame) {
    COMMFrameListElement *newElemPtr = (COMMFrameListElement *)malloc(sizeof(COMMFrameListElement));
    if (!newElemPtr) return;

    newElemPtr->frame = frame;
    newElemPtr->frame.actualText = copyTextString(&frame.actualText);

    if (frame.actualText.string && !newElemPtr->frame.actualText.string) {
        free(newElemPtr);
        return;
    }

    newElemPtr->frame.contentDescript = copyTextString(&frame.contentDescript);

    if (frame.contentDescript.string && !newElemPtr->frame.contentDescript.string) {
        freeTextString(&newElemPtr->frame.actualText);
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

    freeTextString(&toDelete->frame.contentDescript);
    freeTextString(&toDelete->frame.actualText);
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

ID3v2COMMFrameType ListCOMM_getActive(COMMFrameList list) {
    return list.active->frame;
}
