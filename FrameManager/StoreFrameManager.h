#ifndef STOREfRAMEMANAGER_H
#define STOREfRAMEMANAGER_H
#include <stdio.h>
#include "../ID3v2.h"
#include "../ID3v2Frames.h"

void StoreFrame_Header(FILE*, ID3v2FrameHeaderType*);

bool StoreFrame_storeNextFrameInStruct(FILE*, ID3TagType*);

void StoreFrame_APIC(FILE*, uint32_t, ID3v2APICFrameType*);

void StoreFrame_TXTF(FILE*, uint32_t,ID3v2TXTFrameType *);

void StoreFrame_COMM(FILE *, uint32_t , ID3v2COMMFrameType *);

void StoreFrame_PRIV(FILE *, uint32_t , ID3v2PRIVFrameType *);

void StoreFrame_MDCI(FILE*, uint32_t, ID3v2MCDIFrameType*);

void StoreFrame_POPM(FILE*, uint32_t, ID3v2POPMFrameType*);

void StoreFrame_WWWF(FILE*, uint32_t,ID3v2WWWFrameType *);

void StoreFrame_TXXX(FILE *, uint32_t ,ID3v2TXXXFrameType *);

void StoreFrame_WXXX(FILE*, uint32_t,ID3v2WXXXFrameType *);

void StoreFrame_DefaultFrame(FILE*, uint32_t, ID3v2DefaultFrameType*);

void StoreFrame_PCNT(FILE*, uint32_t, ID3v2PCNTFrameType*);

#endif //STOREfRAMEMANAGER_H
