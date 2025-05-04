#ifndef ADDFRAMEMANAGER_H
#define ADDFRAMEMANAGER_H
#include "../ID3v2.h"
#include "../ListFramePtr.h"

int AddFrame_addTXXX(ListFramePtr *,int);

int AddFrame_addTXTF(ListFramePtr *,char *,int );

int AddFrame_addWWWF(ListFramePtr *,char *,int );

int AddFrame_addWXXX(ListFramePtr *,int);

int AddFrame_addAPIC(ListFramePtr *,int );

int AddFrame_addCOMM(ListFramePtr *,int );

int AddFrame_addPOPM(ListFramePtr *,int );

int AddFrame_addPCNT(ID3v2PCNTFrameType **,int );

int AddFrame_addIPLS(ID3v2IPLSFrameType **,int );

#endif // ADDFRAMEMANAGER_H
