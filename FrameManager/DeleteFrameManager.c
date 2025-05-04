#include "DeleteFrameManager.h"
#include "PrintFrameManager.h"
#include "../ID3v2Header.h"
#include <string.h>

#define DELETE_FRAME_LIST(FrameList,FramePtr,FrameType,FreeFunct,PrintFunct)\
    char option;\
    int deletedSize = 0;\
    ListFramePtr_setFirstActive(FrameList);\
    FrameType * FramePtr = ListFramePtr_getActiveFramePtr(*FrameList);\
    do{\
        fflush(stdout);\
        system("clear");\
        PrintFunct(*FramePtr,version);\
        printf("\n\nWant to delete this frame? (y/n): ");\
        do{\
            option = getchar();\
            cleanInputBuffer();\
        }while(option != 'y' && option != 'Y' && option != 'n' && option != 'N');\
        if(option == 'y' || option == 'Y') { \
            deletedSize += FramesV2_getFrameSize(version,FramePtr->header)+10;\
            FreeFunct(&FramePtr);\
            ListFramePtr_deleteActive(FrameList);\
        } \
        else ListFramePtr_setNextActive(FrameList);\
        FramePtr = ListFramePtr_getActiveFramePtr(*FrameList);\
    }while (FramePtr != NULL);\
    return deletedSize;

#define DELETE_FRAME(FramePtr,FreeFunct,PrintFunct)\
    char option;\
    int deletedSize = 0;\
    fflush(stdout);\
    system("clear");\
    PrintFunct(**FramePtr,version);\
    printf("\n\nWant to delete this frame? (y/n): ");\
    do{\
        option = getchar();\
        cleanInputBuffer();\
    }while(option != 'y' && option != 'Y' && option != 'n' && option != 'N');\
    if(option == 'y' || option == 'Y') { \
        deletedSize += FramesV2_getFrameSize(version,(*FramePtr)->header)+10;\
        FreeFunct(FramePtr);\
    } \
    return deletedSize;

static void cleanInputBuffer(){
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
}

int DeleteFrame_deleteTXXX(ListFramePtr *TXXXFrameList,int version){
    DELETE_FRAME_LIST(TXXXFrameList,TXXXFramePtr,ID3v2TXXXFrameType,FramesV2_freeTXXX,PrintFrame_TXXX);
}

int DeleteFrame_deleteTXTF(ListFramePtr *TXTFrameList, char *frameID, int version){
    char option;
    int deletedSize = 0;
    ListFramePtr_setFirstActive(TXTFrameList);
        ID3v2TXTFrameType * TXTFramePtr = ListFramePtr_getActiveFramePtr(*TXTFrameList);
        while (TXTFramePtr != NULL && strncasecmp(frameID,TXTFramePtr->header.frameId,4) != 0){
            ListFramePtr_setNextActive(TXTFrameList);
            TXTFramePtr = ListFramePtr_getActiveFramePtr(*TXTFrameList);
        }
        if(TXTFramePtr != NULL){
            fflush(stdout);
            system("clear");
            PrintFrame_TXTF(*TXTFramePtr,version);
            printf("\n\nWant to delete this frame? (y/n): ");
            do{
                option = getchar();
                cleanInputBuffer();
            }while(option != 'y' && option != 'Y' && option != 'n' && option != 'N');
            if(option == 'y' || option == 'Y') { 
                deletedSize += FramesV2_getFrameSize(version,TXTFramePtr->header)+10;
                FramesV2_freeTXTF(&TXTFramePtr);
                ListFramePtr_deleteActive(TXTFrameList);
            } 
        } 
        return deletedSize;
}
int DeleteFrame_deletePRIV(ListFramePtr *PRIVFrameList,int version){
    DELETE_FRAME_LIST(PRIVFrameList,PRIVFramePtr,ID3v2PRIVFrameType,FramesV2_freePRIV,PrintFrame_PRIV);
}

int DeleteFrame_deleteCOMM(ListFramePtr *COMMFrameList,int version){
    DELETE_FRAME_LIST(COMMFrameList,COMMFramePtr,ID3v2COMMFrameType,FramesV2_freeCOMM,PrintFrame_COMM);
}
int DeleteFrame_deleteAPIC(ListFramePtr *APICFrameList, int version){
    DELETE_FRAME_LIST(APICFrameList,APICFramePtr,ID3v2APICFrameType,FramesV2_freeAPIC,PrintFrame_APIC);
}
int DeleteFrame_deletePOPM(ListFramePtr *POPMFrameList, int version){
    DELETE_FRAME_LIST(POPMFrameList,POPMFramePtr,ID3v2POPMFrameType,FramesV2_freePOPM,PrintFrame_POPM);
}

int DeleteFrame_deleteMDCI(ID3v2MCDIFrameType **MDCIptr, int version){
    DELETE_FRAME(MDCIptr,FramesV2_freeMCDI,PrintFrame_MDCI);
}
int DeleteFrame_deletePCNT(ID3v2PCNTFrameType **PCNTptr, int version){
    DELETE_FRAME(PCNTptr,FramesV2_freePCNT,PrintFrame_PCNT);
}

int DeleteFrame_deleteWXXX(ListFramePtr *WXXXFrameList,int version){
    DELETE_FRAME_LIST(WXXXFrameList,WXXXFramePtr,ID3v2WXXXFrameType,FramesV2_freeWXXX,PrintFrame_WXXX);
}

int DeleteFrame_deleteWWWF(ListFramePtr *WWWFrameList, char *frameID, int version){
    char option;
    int deletedSize = 0;
    ListFramePtr_setFirstActive(WWWFrameList);
        ID3v2WWWFrameType * WWWFramePtr = ListFramePtr_getActiveFramePtr(*WWWFrameList);
        while (WWWFramePtr != NULL && strncasecmp(frameID,WWWFramePtr->header.frameId,4) != 0){
            ListFramePtr_setNextActive(WWWFrameList);
            WWWFramePtr = ListFramePtr_getActiveFramePtr(*WWWFrameList);
        }
        if(WWWFramePtr != NULL){
            fflush(stdout);
            system("clear");
            PrintFrame_WWWF(*WWWFramePtr,version);
            printf("\n\nWant to delete this frame? (y/n): ");
            do{
                option = getchar();
                cleanInputBuffer();
            }while(option != 'y' && option != 'Y' && option != 'n' && option != 'N');
            if(option == 'y' || option == 'Y') { 
                deletedSize += FramesV2_getFrameSize(version,WWWFramePtr->header)+10;
                FramesV2_freeWWWF(&WWWFramePtr);
                ListFramePtr_deleteActive(WWWFrameList);
            } 
        } 
        return deletedSize;
}

int DeleteFrame_IPLS(ID3v2IPLSFrameType **IPLSptr, int version){
    DELETE_FRAME(IPLSptr,FramesV2_freeIPLS,PrintFrame_IPLS);
}

int DeleteFrame_default(ID3v2DefaultFrameType **DefaultFramePtr,int version){
    DELETE_FRAME(DefaultFramePtr,FramesV2_freeDefaultFrame,PrintFrame_DefaultFrame);
}
int DeleteFrame_defaultList(ListFramePtr *DefaultFrameList,int version){
    DELETE_FRAME_LIST(DefaultFrameList,DefaultFramePtr,ID3v2DefaultFrameType,FramesV2_freeDefaultFrame,PrintFrame_DefaultFrame);
}