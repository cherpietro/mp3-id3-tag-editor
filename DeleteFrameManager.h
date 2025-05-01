#ifndef DELETEFRAMEMANAGER_H
#define DELETEFRAMEMANAGER_H
#include "ListFramePtr.h"

void DeleteFrame_deleteTXXX(ListFramePtr *);
void DeleteFrame_deleteTXTF(ListFramePtr *,char *);
void DeleteFrame_deletePRIV(ListFramePtr *);
void DeleteFrame_deleteCOMM(ListFramePtr *);
void DeleteFrame_deleteAPIC(ListFramePtr *);
void DeleteFrame_deletePOPM(ListFramePtr *);

#endif // DELETEFRAMEMANAGER_H
