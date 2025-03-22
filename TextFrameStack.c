#include "TextFrameStack.h"
#include <string.h> 
void initTextFrameStack(TextFrameStack *stack){
  stack->top = NULL;
}

void pushTextFrameStack(TextFrameStack *stack, ID3v2TextFrameType frame){
  // check the result of operation malloc!
  TextFrameStackElement *newElemPtr = (TextFrameStackElement *)malloc(sizeof(TextFrameStackElement));
  newElemPtr->frame = frame;
  if (frame.content) {
    newElemPtr->frame.content = strdup(frame.content);
    if (!newElemPtr->frame.content) {
      free(newElemPtr);
      return;
    }
  } else {
    newElemPtr->frame.content = NULL;
  }  
  newElemPtr->next = stack->top;
  stack->top = newElemPtr;
}

void popTextFrameStack(TextFrameStack *stack){
  TextFrameStackElement *elemPtr;
  // free(stack->top->frame.content);
  if (stack->top != NULL) {
    elemPtr = stack->top;
    stack->top = stack->top->next;
    free(elemPtr);
  }
}

ID3v2TextFrameType topTextFrameStack(TextFrameStack stack){
  return stack.top->frame;
}

bool isEmptyTextFrameStack(TextFrameStack *stack){
  return (stack->top == NULL);
}

void freeTextFrameStack(TextFrameStack *stack){
  while(!isEmptyTextFrameStack(stack)){
    popTextFrameStack(stack);
  }
}
