#include "ID3v2Frames.h"

ID3v2APICFrameType* GetFrame_APIC(int);
ID3v2TXTFrameType* GetFrame_TXXX(int);
ID3v2TXTFrameType* GetFrame_TXTF(char *,int);
ID3v2WWWFrameType* GetFrame_WWWF(char *,int);
ID3v2WXXXFrameType* GetFrame_WXXX(int);
bool FramesV2_validTextFrameId(char *);
bool FramesV2_validWebFrameId(char *);
