#ifndef ADDFRAMEMANAGER_H
#define ADDFRAMEMANAGER_H
#include "ID3v2.h"
#include "ListFramePtr.h"

int AddFrame_addTXXX(ListFramePtr *,int);

int AddFrame_addTXTF(ListFramePtr *,char *,int );

int AddFrame_addWWWF(ListFramePtr *,char *,int );

int AddFrame_addWXXX(ListFramePtr *,int);

int AddFrame_addAPIC(ListFramePtr *,int );

#endif // ADDFRAMEMANAGER_H
