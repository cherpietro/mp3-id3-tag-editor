#ifndef DELETEFRAMEMANAGER_H
#define DELETEFRAMEMANAGER_H
#include "../ListFramePtr.h"

int DeleteFrame_deleteTXXX(ListFramePtr *,int);
int DeleteFrame_deleteTXTF(ListFramePtr *,char *,int);
int DeleteFrame_deletePRIV(ListFramePtr *,int);
int DeleteFrame_deleteCOMM(ListFramePtr *,int);
int DeleteFrame_deleteAPIC(ListFramePtr *,int);
int DeleteFrame_deletePOPM(ListFramePtr *,int);
int DeleteFrame_deleteMDCI(ID3v2MCDIFrameType **, int);
int DeleteFrame_deleteWXXX(ListFramePtr *,int);
int DeleteFrame_deleteWWWF(ListFramePtr *,char *,int);

int DeleteFrame_default(ID3v2DefaultFrameType **,int);
int DeleteFrame_defaultList(ListFramePtr *,int);

#endif // DELETEFRAMEMANAGER_H
