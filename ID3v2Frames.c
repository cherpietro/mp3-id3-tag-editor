#include "ID3v2Frames.h"
#include "SizeReader.h"
#include <string.h>

uint32_t FramesV2_getFrameSize(int version, ID3v2FrameHeaderType header){
    if(version == 4) return syncsafeToSize(header.size);
    else return sizeFromID3v23(header.size);
}
void FramesV2_updateFrameSize(int version, ID3v2FrameHeaderType *header,uint32_t newSize){
    if(version == 4) saveSizeToSyncsafe(newSize,header->size);
    else saveSizeToID3v23(newSize,header->size);
}

void FramesV2_freeAPIC(ID3v2APICFrameType** APIC){
    TxtStr_freeTextString(&(*APIC)->mimeType);
    TxtStr_freeTextString(&(*APIC)->description);
    if((*APIC)->imageData != NULL) free((*APIC)->imageData);
    free(*APIC);
    *APIC = NULL;
}

void FramesV2_freeTXTF(ID3v2TXTFrameType** TXTF){
    TxtStr_freeTextString(&(*TXTF)->content);
    free(*TXTF);
    *TXTF = NULL;
}
void FramesV2_freeWXXX(ID3v2WXXXFrameType **WXXX){
    TxtStr_freeTextString(&(*WXXX)->description);
    TxtStr_freeTextString(&(*WXXX)->url);
    free(*WXXX);
    *WXXX = NULL;
}
void FramesV2_freeCOMM(ID3v2COMMFrameType** COMM){
    TxtStr_freeTextString(&(*COMM)->actualText);
    TxtStr_freeTextString(&(*COMM)->contentDescript);
    free(*COMM);
    *COMM = NULL;
}
void FramesV2_freePRIV(ID3v2PRIVFrameType** PRIV){
    TxtStr_freeTextString(&(*PRIV)->owner);
    TxtStr_freeTextString(&(*PRIV)->privateData);
    free(*PRIV);
    *PRIV = NULL;
}
void FramesV2_freeMCDI(ID3v2MCDIFrameType **MCDI){
    TxtStr_freeTextString(&(*MCDI)->CDTOC);
    free(*MCDI);
    *MCDI = NULL;
}
void FramesV2_freePOPM(ID3v2POPMFrameType **POPM){
    TxtStr_freeTextString(&(*POPM)->userEmail);
    free(*POPM);
    *POPM = NULL;
}
void FramesV2_freeWWWF(ID3v2WWWFrameType **WWWF){
    TxtStr_freeTextString(&(*WWWF)->url);
    free(*WWWF);
    *WWWF = NULL;
}
void FramesV2_freeDefaultFrame(ID3v2DefaultFrameType **DefaultFrame){
    if((*DefaultFrame)->frameData != NULL) free((*DefaultFrame)->frameData);
    free(*DefaultFrame);
    *DefaultFrame = NULL;
}
