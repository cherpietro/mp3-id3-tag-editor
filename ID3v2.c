#include "ID3v2.h"
#include "SizeReader.h"
#include "FrameManager/FileFrameManager.h"
#include "FrameManager/PrintFrameManager.h"
#include "FrameManager/DeleteFrameManager.h"
#include "FrameManager/AddFrameManager.h"
#include "FrameManager/StoreFrameManager.h"
#include "FrameManager/SizeFrameManager.h"
#include <string.h>
#include "FrameManager/GetFrameManager.h"


#define FREE_FRAME_LIST(frameList,frameType,freeFrameFunc)\
    ListFramePtr_setFirstActive(&ID3Tag->frameList);\
    while (!ListFramePtr_isEmpty(ID3Tag->frameList)) {\
        frameType* framePtr = (frameType*) ListFramePtr_getActiveFramePtr(ID3Tag->frameList);\
        freeFrameFunc(&framePtr);\
        ListFramePtr_deleteActive(&ID3Tag->frameList);\
    }
#define ADD_FRAME_LIST(frameList,addFunction)\
    incrementedSize = addFunction(&ID3Tag->frameList,ID3Tag->header.version[0]);\
    oldSize = HeaderV2_getTagSize(ID3Tag->header);\
    HeaderV2_updateTagSize(&ID3Tag->header,oldSize+incrementedSize);

#define ADD_SINGLE_FRAME_LIST(frameList,addFunction,frameID)\
    incrementedSize = addFunction(&ID3Tag->frameList,frameID,ID3Tag->header.version[0]);\
    oldSize = HeaderV2_getTagSize(ID3Tag->header);\
    HeaderV2_updateTagSize(&ID3Tag->header,oldSize+incrementedSize);

void ID3v2_init(ID3TagType *ID3Tag){
    ListFramePtr_init(&ID3Tag->TXTFrameList);
    ListFramePtr_init(&ID3Tag->COMMFrameList);
    ListFramePtr_init(&ID3Tag->PRIVFrameList);
    ListFramePtr_init(&ID3Tag->APICFrameList);
    ListFramePtr_init(&ID3Tag->POPMFrameList);
    ListFramePtr_init(&ID3Tag->WWWFrameList);
    ListFramePtr_init(&ID3Tag->WXXXFrameList);
    ListFramePtr_init(&ID3Tag->UFIDFrameList);
    ListFramePtr_init(&ID3Tag->USLTFrameList);
    ListFramePtr_init(&ID3Tag->SYLTFrameList);
    ListFramePtr_init(&ID3Tag->GEOBFrameList);
    ListFramePtr_init(&ID3Tag->LINKFrameList);
    ListFramePtr_init(&ID3Tag->AENCFrameList);
    ListFramePtr_init(&ID3Tag->ENCRFrameList);
    ListFramePtr_init(&ID3Tag->GRIDFrameList);

    ID3Tag->MCDI = NULL;
    ID3Tag->RVRB = NULL;
    ID3Tag->POSS = NULL;
    ID3Tag->COMR = NULL;
    ID3Tag->EQUA = NULL;
    ID3Tag->MLLT = NULL;
    ID3Tag->ETCO = NULL;
    ID3Tag->RVAD = NULL;

    ID3Tag->IPLS = NULL;
    ID3Tag->SYTC = NULL;
    ID3Tag->USER = NULL;
    ID3Tag->OWNE = NULL;
    ID3Tag->PCNT = NULL;
}

void ID3v2_free(ID3TagType *ID3Tag){
    FREE_FRAME_LIST(TXTFrameList,ID3v2TXTFrameType,FramesV2_freeTXTF);
    FREE_FRAME_LIST(APICFrameList,ID3v2APICFrameType,FramesV2_freeAPIC);
    FREE_FRAME_LIST(WWWFrameList,ID3v2WWWFrameType,FramesV2_freeWWWF);
    FREE_FRAME_LIST(WXXXFrameList,ID3v2WXXXFrameType,FramesV2_freeWXXX);
    FREE_FRAME_LIST(COMMFrameList,ID3v2COMMFrameType,FramesV2_freeCOMM);
    FREE_FRAME_LIST(PRIVFrameList,ID3v2PRIVFrameType,FramesV2_freePRIV);
    FREE_FRAME_LIST(POPMFrameList,ID3v2POPMFrameType,FramesV2_freePOPM);
    if(ID3Tag->MCDI != NULL) FramesV2_freeMCDI(&ID3Tag->MCDI);

    FREE_FRAME_LIST(UFIDFrameList,ID3v2DefaultFrameType,FramesV2_freeDefaultFrame);
    FREE_FRAME_LIST(USLTFrameList,ID3v2DefaultFrameType,FramesV2_freeDefaultFrame);
    FREE_FRAME_LIST(SYLTFrameList,ID3v2DefaultFrameType,FramesV2_freeDefaultFrame);
    FREE_FRAME_LIST(GEOBFrameList,ID3v2DefaultFrameType,FramesV2_freeDefaultFrame);
    FREE_FRAME_LIST(LINKFrameList,ID3v2DefaultFrameType,FramesV2_freeDefaultFrame);
    FREE_FRAME_LIST(AENCFrameList,ID3v2DefaultFrameType,FramesV2_freeDefaultFrame);
    FREE_FRAME_LIST(ENCRFrameList,ID3v2DefaultFrameType,FramesV2_freeDefaultFrame);
    FREE_FRAME_LIST(GRIDFrameList,ID3v2DefaultFrameType,FramesV2_freeDefaultFrame);
    if(ID3Tag->COMR != NULL) FramesV2_freeDefaultFrame(&ID3Tag->COMR);
    if(ID3Tag->POSS != NULL) FramesV2_freeDefaultFrame(&ID3Tag->POSS);
    if(ID3Tag->RVRB != NULL) FramesV2_freeDefaultFrame(&ID3Tag->RVRB);
    if(ID3Tag->EQUA != NULL) FramesV2_freeDefaultFrame(&ID3Tag->EQUA);
    if(ID3Tag->MLLT != NULL) FramesV2_freeDefaultFrame(&ID3Tag->MLLT);
    if(ID3Tag->ETCO != NULL) FramesV2_freeDefaultFrame(&ID3Tag->ETCO);
    if(ID3Tag->RVAD != NULL) FramesV2_freeDefaultFrame(&ID3Tag->RVAD);
    if(ID3Tag->IPLS != NULL) FramesV2_freeDefaultFrame(&ID3Tag->IPLS);
    if(ID3Tag->SYTC != NULL) FramesV2_freeDefaultFrame(&ID3Tag->SYTC);
    if(ID3Tag->USER != NULL) FramesV2_freeDefaultFrame(&ID3Tag->USER);
    if(ID3Tag->OWNE != NULL) FramesV2_freeDefaultFrame(&ID3Tag->OWNE);
    if(ID3Tag->PCNT != NULL) FramesV2_freePCNT(&ID3Tag->PCNT);
}

int ID3v2_storeTagInStruct(char *file,ID3TagType *ID3Tag){
    char *extension = strrchr(file, '.');
    if (extension == NULL || strcmp(extension, ".mp3") != 0) {
        printf("Not .mp3 file!\n");
        return -1;
    }
    FILE *mp3FilePointer = fopen(file,"r");
    if (mp3FilePointer) {
        int paddingReached = 0;
        HeaderV2_storeHeader(mp3FilePointer,&ID3Tag->header);
        if(HeaderV2_isID3v2Tag(ID3Tag->header)){
                HeaderV2_printTagHeader(ID3Tag->header);
                uint32_t tagSize = HeaderV2_getTagSize(ID3Tag->header);
                while(!paddingReached && ftell(mp3FilePointer) < tagSize + 10){
                    paddingReached = StoreFrame_storeNextFrameInStruct(mp3FilePointer,ID3Tag);
                }
                if (paddingReached) ID3Tag->paddingSize = (HeaderV2_getTagSize(ID3Tag->header))+10 - (ftell(mp3FilePointer))+10; //ID3Tag size doesn't include header 
                return 0;
        }
        else printf("Not ID3v2 Tag detected or not yet supported version\n");
        fclose(mp3FilePointer);
        return -1;
    }
    else printf("The file DOESN'T exist!\n");
    return -1;
}

//IMPLEMENT REMAINING TAGS
bool ID3v2_getTagSizeOfTheStruct(ID3TagType *ID3Tag){
    size_t tagSizeOfStruct;
    printf("\nsize in ID3Tag: %d bytes\n",HeaderV2_getTagSize(ID3Tag->header));
    tagSizeOfStruct = SizeFrame_getFrameListSize(ID3Tag);    
    if((HeaderV2_getTagSize(ID3Tag->header) +10 - ID3Tag->paddingSize ) == tagSizeOfStruct ){
        return true;
        // printf("size is okay\n");
    }
    else{
        printf("size is NOT okay %ld\n",(tagSizeOfStruct+10 - (int) ID3Tag->paddingSize ));
        return false;
    }
}

void ID3v2_deleteFrame(ID3TagType *ID3Tag, char *frameID){
    int deletedSize;
    int oldSize;
    if(strncasecmp(frameID,"TXXX",4)==0) deletedSize = DeleteFrame_deleteTXXX(&ID3Tag->TXTFrameList,ID3Tag->header.version[0]);
    else if(strncasecmp(frameID,"T",1)==0) deletedSize = DeleteFrame_deleteTXTF(&ID3Tag->TXTFrameList,frameID,ID3Tag->header.version[0]);
    else if(strncasecmp(frameID,"WXXX",4)==0) deletedSize = DeleteFrame_deleteWXXX(&ID3Tag->WXXXFrameList,ID3Tag->header.version[0]);
    else if(strncasecmp(frameID,"W",1)==0) deletedSize = DeleteFrame_deleteWWWF(&ID3Tag->WWWFrameList,frameID,ID3Tag->header.version[0]);
    else if(strncasecmp(frameID,"PRIV",4)==0) deletedSize = DeleteFrame_deletePRIV(&ID3Tag->PRIVFrameList,ID3Tag->header.version[0]);
    else if(strncasecmp(frameID,"COMM",4)==0) deletedSize = DeleteFrame_deleteCOMM(&ID3Tag->COMMFrameList,ID3Tag->header.version[0]);
    else if(strncasecmp(frameID,"APIC",4)==0) deletedSize = DeleteFrame_deleteAPIC(&ID3Tag->APICFrameList,ID3Tag->header.version[0]);
    else if(strncasecmp(frameID,"POPM",4)==0) deletedSize = DeleteFrame_deletePOPM(&ID3Tag->POPMFrameList,ID3Tag->header.version[0]);
    else if(strncasecmp(frameID,"MDCI",4)==0) deletedSize = DeleteFrame_deleteMDCI(&ID3Tag->MCDI,ID3Tag->header.version[0]);
    //NOT TESTED
    else if(strncasecmp(frameID,"RVRB",4)==0){deletedSize = DeleteFrame_default(&ID3Tag->RVRB,ID3Tag->header.version[0]);}
    else if(strncasecmp(frameID,"EQUA",4)==0){deletedSize = DeleteFrame_default(&ID3Tag->EQUA,ID3Tag->header.version[0]);}
    else if(strncasecmp(frameID,"MLLT",4)==0){deletedSize = DeleteFrame_default(&ID3Tag->MLLT,ID3Tag->header.version[0]);}
    else if(strncasecmp(frameID,"ETCO",4)==0){deletedSize = DeleteFrame_default(&ID3Tag->ETCO,ID3Tag->header.version[0]);}
    else if(strncasecmp(frameID,"RVAD",4)==0){deletedSize = DeleteFrame_default(&ID3Tag->RVAD,ID3Tag->header.version[0]);}
    else if(strncasecmp(frameID,"POSS",4)==0){deletedSize = DeleteFrame_default(&ID3Tag->POSS,ID3Tag->header.version[0]);}
    else if(strncasecmp(frameID,"COMR",4)==0){deletedSize = DeleteFrame_default(&ID3Tag->COMR,ID3Tag->header.version[0]);}
    else if(strncasecmp(frameID,"UFID",4)==0){deletedSize = DeleteFrame_defaultList(&ID3Tag->UFIDFrameList,ID3Tag->header.version[0]);}
    else if(strncasecmp(frameID,"USLT",4)==0){deletedSize = DeleteFrame_defaultList(&ID3Tag->USLTFrameList,ID3Tag->header.version[0]);}
    else if(strncasecmp(frameID,"SYLT",4)==0){deletedSize = DeleteFrame_defaultList(&ID3Tag->SYLTFrameList,ID3Tag->header.version[0]);}
    else if(strncasecmp(frameID,"GEOB",4)==0){deletedSize = DeleteFrame_defaultList(&ID3Tag->GEOBFrameList,ID3Tag->header.version[0]);}
    else if(strncasecmp(frameID,"LINK",4)==0){deletedSize = DeleteFrame_defaultList(&ID3Tag->LINKFrameList,ID3Tag->header.version[0]);}
    else if(strncasecmp(frameID,"AENC",4)==0){deletedSize = DeleteFrame_defaultList(&ID3Tag->AENCFrameList,ID3Tag->header.version[0]);}
    else if(strncasecmp(frameID,"ENCR",4)==0){deletedSize = DeleteFrame_defaultList(&ID3Tag->ENCRFrameList,ID3Tag->header.version[0]);}
    else if(strncasecmp(frameID,"GRID",4)==0){deletedSize = DeleteFrame_defaultList(&ID3Tag->GRIDFrameList,ID3Tag->header.version[0]);}
    else if(strncasecmp(frameID,"IPLS",4)==0){deletedSize = DeleteFrame_default(&ID3Tag->IPLS,ID3Tag->header.version[0]);}
    else if(strncasecmp(frameID,"SYTC",4)==0){deletedSize = DeleteFrame_default(&ID3Tag->SYTC,ID3Tag->header.version[0]);}
    else if(strncasecmp(frameID,"USER",4)==0){deletedSize = DeleteFrame_default(&ID3Tag->USER,ID3Tag->header.version[0]);}
    else if(strncasecmp(frameID,"OWNE",4)==0){deletedSize = DeleteFrame_default(&ID3Tag->OWNE,ID3Tag->header.version[0]);}
    else if(strncasecmp(frameID,"PCNT",4)==0){deletedSize = DeleteFrame_deletePCNT(&ID3Tag->PCNT,ID3Tag->header.version[0]);}
    else return; //NOT SUPPORTED TAG
    oldSize = HeaderV2_getTagSize(ID3Tag->header);
    HeaderV2_updateTagSize(&ID3Tag->header,oldSize-deletedSize);

}

void ID3v2_addFrame(ID3TagType *ID3Tag, char *frameID){
    int incrementedSize = 0;
    int oldSize = 0;
    if(strncasecmp(frameID,"TXXX",4)==0){ADD_FRAME_LIST(TXTFrameList,AddFrame_addTXXX);}    
    else if(strncasecmp(frameID,"T",1)==0){ 
        if(FramesV2_validTextFrameId(frameID)){ ADD_SINGLE_FRAME_LIST(TXTFrameList, AddFrame_addTXTF, frameID);}
        else printf("Not supported tag to include");
    }
    else if(strncasecmp(frameID,"WXXX",4)==0){ADD_FRAME_LIST(WXXXFrameList,AddFrame_addWXXX);}
    else if(strncasecmp(frameID,"W",1)==0){
        if(FramesV2_validWebFrameId(frameID)){ADD_SINGLE_FRAME_LIST(WWWFrameList, AddFrame_addWWWF, frameID);}
        else printf("Not supported tag to include");
    }
    else if(strncasecmp(frameID,"APIC",4)==0){ADD_FRAME_LIST(APICFrameList, AddFrame_addAPIC);}
    else if(strncasecmp(frameID,"COMM",4)==0){ADD_FRAME_LIST(COMMFrameList, AddFrame_addCOMM);}
    else if(strncasecmp(frameID,"POPM",4)==0){ADD_FRAME_LIST(POPMFrameList, AddFrame_addPOPM);}
    else if(strncasecmp(frameID,"PCNT",4)==0){ADD_FRAME_LIST(PCNT, AddFrame_addPCNT);}
}
