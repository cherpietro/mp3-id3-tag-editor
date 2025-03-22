#include "TextFrameList.h"
#include <string.h> 
void initTextFrameList(TextFrameList *list){
  list->first = NULL;
  list->last = NULL;
  list->active = NULL;
}

void insertLastTextFrameList(TextFrameList *list, ID3v2TextFrameType frame){
  // check the result of operation malloc!
  TextFrameListElement *newElemPtr = (TextFrameListElement *)malloc(sizeof(TextFrameListElement));
  newElemPtr->frame = frame;
  newElemPtr->frame.content = strdup(frame.content);
  if (!newElemPtr->frame.content) {
    free(newElemPtr);
    return;
  }
  if(list->first == NULL && list->first == NULL){
    newElemPtr->next = newElemPtr;
    newElemPtr->prev = newElemPtr;

    list->first = newElemPtr;
    list->last = newElemPtr;
  }
  else{
    newElemPtr->next = list->first;
    newElemPtr->prev = list->last;

    list->last->next = newElemPtr;
    list->first->prev = newElemPtr;
    list->last = newElemPtr;
  }
}

void setFirstActiveTextFrameList(TextFrameList *list){
  list->active = list->first;
}
void setNextActiveTextFramelist(TextFrameList *list){
  list->active = list->active->next;
}

void deleteActiveTextFrameList(TextFrameList *list){
  if (list->active != NULL) {
    if(list->active == list->first &&list->active == list->last ){
      free(list->active->frame.content);
      free(list->active);
      list->first = NULL;
      list->last = NULL;
      list->active = NULL;
      return;
    }
    else if(list->active == list->first){
      list->first = list->first->next;
      
      list->first->prev = list->last;
      list->last->next = list->first;
    }
    else if(list->active == list->last){
      list->last = list->last->prev;
  
      list->last->next = list->first;
      list->first->prev = list->last;
    }
    else{
      list->active->prev->next =  list->active->next;
      list->active->next->prev =  list->active->prev;
    }
    list->active->next = NULL;
    list->active->prev = NULL;
    free(list->active);
    list->active = NULL;
  }
}

ID3v2TextFrameType getTextFrameListActive(TextFrameList list){
  return list.active->frame;
}

bool isEmptyTextFrameList(TextFrameList list){
  return (list.first == NULL && list.last == NULL);
}

void freeTextFrameList(TextFrameList *list){
  while(!isEmptyTextFrameList(*list)){
    setFirstActiveTextFrameList(list);
    deleteActiveTextFrameList(list);
  }
}
