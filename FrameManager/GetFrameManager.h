#include "../ID3v2Frames.h"

ID3v2APICFrameType* GetFrame_APIC(int);
ID3v2TXXXFrameType* GetFrame_TXXX(int);
ID3v2TXTFrameType* GetFrame_TXTF(char *,int);
ID3v2WWWFrameType* GetFrame_WWWF(char *,int);
ID3v2WXXXFrameType* GetFrame_WXXX(int);
ID3v2POPMFrameType* GetFrame_POPM(int);
ID3v2PCNTFrameType* GetFrame_PCNT(int);
ID3v2COMMFrameType* GetFrame_COMM(int);

ID3v2IPLSFrameType* GetFrame_IPLS(int);

bool FramesV2_validTextFrameId(char *);
bool FramesV2_validWebFrameId(char *);
