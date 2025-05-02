#ifndef DELETEFRAMEMANAGER_H
#define DELETEFRAMEMANAGER_H
#include "ListFramePtr.h"

int DeleteFrame_deleteTXXX(ListFramePtr *,int);
int DeleteFrame_deleteTXTF(ListFramePtr *,char *,int);
int DeleteFrame_deletePRIV(ListFramePtr *,int);
int DeleteFrame_deleteCOMM(ListFramePtr *,int);
int DeleteFrame_deleteAPIC(ListFramePtr *,int);
int DeleteFrame_deletePOPM(ListFramePtr *,int);

#endif // DELETEFRAMEMANAGER_H
