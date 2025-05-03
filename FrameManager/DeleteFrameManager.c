#include "DeleteFrameManager.h"
#include "PrintFrameManager.h"
#include "../ID3v2Header.h"
#include <string.h>

static void cleanInputBuffer(){
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
}

int DeleteFrame_deleteTXXX(ListFramePtr *TXTFrameList,int version){
    char option;
    int deletedSize = 0;
    ListFramePtr_setFirstActive(TXTFrameList);
        ID3v2TXTFrameType * TXTFramePtr = ListFramePtr_getActiveFramePtr(*TXTFrameList);
        do{
            if(strncasecmp(TXTFramePtr->header.frameId,"TXXX",4)==0){
                fflush(stdout);
                system("clear");
                PrintFrame_TXTF(*TXTFramePtr);
                printf("\n\nWant to delete this frame? (y/n): ");
                option = getchar();
                cleanInputBuffer();
                if(option == 'y'){
                    deletedSize += FramesV2_getFrameSize(version,TXTFramePtr->header)+10;
                    FramesV2_freeTXTF(&TXTFramePtr);
                    ListFramePtr_deleteActive(TXTFrameList);
                } 
                else ListFramePtr_setNextActive(TXTFrameList);
            }
            else ListFramePtr_setNextActive(TXTFrameList);
            TXTFramePtr = ListFramePtr_getActiveFramePtr(*TXTFrameList);
        }while (TXTFramePtr != NULL);
        return deletedSize;
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
            PrintFrame_TXTF(*TXTFramePtr);
            printf("\n\nWant to delete this frame? (y/n): ");
            option = getchar();
            cleanInputBuffer();
            if(option == 'y') {
                deletedSize += FramesV2_getFrameSize(version,TXTFramePtr->header)+10;
                FramesV2_freeTXTF(&TXTFramePtr);
                ListFramePtr_deleteActive(TXTFrameList);
            } 
        } 
        return deletedSize;
}

int DeleteFrame_deletePRIV(ListFramePtr *PRIVFrameList,int version){
    char option;
    int deletedSize = 0;
    ListFramePtr_setFirstActive(PRIVFrameList);
    ID3v2PRIVFrameType * PRIVFramePtr = ListFramePtr_getActiveFramePtr(*PRIVFrameList);
    do{
        fflush(stdout);
        system("clear");
        PrintFrame_PRIV(*PRIVFramePtr);
        printf("\n\nWant to delete this frame? (y/n): ");
        option = getchar();
        cleanInputBuffer();
        if(option == 'y') {
            deletedSize += FramesV2_getFrameSize(version,PRIVFramePtr->header)+10;
            FramesV2_freePRIV(&PRIVFramePtr);
            ListFramePtr_deleteActive(PRIVFrameList);
        } 
        else ListFramePtr_setNextActive(PRIVFrameList);
        PRIVFramePtr = ListFramePtr_getActiveFramePtr(*PRIVFrameList);
    }while (PRIVFramePtr != NULL);
    return deletedSize;
}

int DeleteFrame_deleteCOMM(ListFramePtr *COMMFrameList,int version){
    char option;  
    int deletedSize = 0;  
    ListFramePtr_setFirstActive(COMMFrameList);
    ID3v2COMMFrameType * COMMFramePtr = ListFramePtr_getActiveFramePtr(*COMMFrameList);
    do{
        fflush(stdout);
        system("clear");
        PrintFrame_COMM(*COMMFramePtr);
        printf("\n\nWant to delete this frame? (y/n): ");
        option = getchar();
        cleanInputBuffer();
        if(option == 'y'){
            deletedSize += FramesV2_getFrameSize(version,COMMFramePtr->header)+10;
            FramesV2_freeCOMM(&COMMFramePtr);
            ListFramePtr_deleteActive(COMMFrameList);
        } 
        else ListFramePtr_setNextActive(COMMFrameList);
        COMMFramePtr = ListFramePtr_getActiveFramePtr(*COMMFrameList);
    }while (COMMFramePtr != NULL);
    return deletedSize;
}
int DeleteFrame_deleteAPIC(ListFramePtr *APICFrameList, int version){
    char option;
    int deletedSize = 0;
    ListFramePtr_setFirstActive(APICFrameList);
    ID3v2APICFrameType * APICFramePtr = ListFramePtr_getActiveFramePtr(*APICFrameList);
    do{
        fflush(stdout);
        system("clear");
        PrintFrame_APIC(*APICFramePtr);
        printf("\n\nWant to delete this frame? (y/n): ");
        option = getchar();
        cleanInputBuffer();
        if(option == 'y') {
            deletedSize += FramesV2_getFrameSize(version,APICFramePtr->header)+10;
            FramesV2_freeAPIC(&APICFramePtr);
            ListFramePtr_deleteActive(APICFrameList);
        } 
        else ListFramePtr_setNextActive(APICFrameList);
        APICFramePtr = ListFramePtr_getActiveFramePtr(*APICFrameList);
    }while (APICFramePtr != NULL);
    return deletedSize;
}
int DeleteFrame_deletePOPM(ListFramePtr *POPMFrameList, int version){
    char option;
    int deletedSize = 0;
    ListFramePtr_setFirstActive(POPMFrameList);
    ID3v2POPMFrameType * POPMFramePtr = ListFramePtr_getActiveFramePtr(*POPMFrameList);
    do{
        fflush(stdout);
        system("clear");
        PrintFrame_POPM(*POPMFramePtr);
        printf("\n\nWant to delete this frame? (y/n): ");
        option = getchar();
        cleanInputBuffer();
        if(option == 'y') {
            deletedSize += FramesV2_getFrameSize(version,POPMFramePtr->header)+10;
            FramesV2_freePOPM(&POPMFramePtr);
            ListFramePtr_deleteActive(POPMFrameList);
        } 
        else ListFramePtr_setNextActive(POPMFrameList);
        POPMFramePtr = ListFramePtr_getActiveFramePtr(*POPMFrameList);
    }while (POPMFramePtr != NULL);
    return deletedSize;
}

int DeleteFrame_deleteMDCI(ID3v2MCDIFrameType **MDCIptr, int version){
    char option;
    int deletedSize = 0;
    fflush(stdout);
    system("clear");
    PrintFrame_MDCI(**MDCIptr);
    printf("\n\nWant to delete this frame? (y/n): ");
    option = getchar();
    cleanInputBuffer();
    if(option == 'y') {
        deletedSize += FramesV2_getFrameSize(version,(*MDCIptr)->header)+10;
        FramesV2_freeMCDI(MDCIptr);
    } 
    return deletedSize;
}

int DeleteFrame_deleteWXXX(ListFramePtr *WXXXFrameList,int version){
    char option;
    int deletedSize = 0;
    ListFramePtr_setFirstActive(WXXXFrameList);
        ID3v2WXXXFrameType * WXXXFramePtr = ListFramePtr_getActiveFramePtr(*WXXXFrameList);
        do{
            fflush(stdout);
            system("clear");
            PrintFrame_WXXX(*WXXXFramePtr);
            printf("\n\nWant to delete this frame? (y/n): ");
            option = getchar();
            cleanInputBuffer();
            if(option == 'y'){
                deletedSize += FramesV2_getFrameSize(version,WXXXFramePtr->header)+10;
                FramesV2_freeWXXX(&WXXXFramePtr);
                ListFramePtr_deleteActive(WXXXFrameList);
            } 
            else ListFramePtr_setNextActive(WXXXFrameList);
            WXXXFramePtr = ListFramePtr_getActiveFramePtr(*WXXXFrameList);
        }while (WXXXFramePtr != NULL);
        return deletedSize;
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
            PrintFrame_WWWF(*WWWFramePtr);
            printf("\n\nWant to delete this frame? (y/n): ");
            option = getchar();
            cleanInputBuffer();
            if(option == 'y') {
                deletedSize += FramesV2_getFrameSize(version,WWWFramePtr->header)+10;
                FramesV2_freeWWWF(&WWWFramePtr);
                ListFramePtr_deleteActive(WWWFrameList);
            } 
        } 
        return deletedSize;
}

int DeleteFrame_default(ID3v2DefaultFrameType **DefaultFramePtr,int version){
    char option;
    int deletedSize = 0;
    fflush(stdout);
    system("clear");
    PrintFrame_DefaultFrame(**DefaultFramePtr);
    printf("\n\nWant to delete this frame? (y/n): ");
    option = getchar();
    cleanInputBuffer();
    if(option == 'y') {
        deletedSize += FramesV2_getFrameSize(version,(*DefaultFramePtr)->header)+10;
        FramesV2_freeDefaultFrame(DefaultFramePtr);
    } 
    return deletedSize;
}
int DeleteFrame_defaultList(ListFramePtr *DefaultFrameList,int version){
    char option;
    int deletedSize = 0;
    ListFramePtr_setFirstActive(DefaultFrameList);
    ID3v2DefaultFrameType * DefaultFramePtr = ListFramePtr_getActiveFramePtr(*DefaultFrameList);
    do{
        fflush(stdout);
        system("clear");
        PrintFrame_DefaultFrame(*DefaultFramePtr);
        printf("\n\nWant to delete this frame? (y/n): ");
        option = getchar();
        cleanInputBuffer();
        if(option == 'y') {
            deletedSize += FramesV2_getFrameSize(version,DefaultFramePtr->header)+10;
            FramesV2_freeDefaultFrame(&DefaultFramePtr);
            ListFramePtr_deleteActive(DefaultFrameList);
        } 
        else ListFramePtr_setNextActive(DefaultFrameList);
        DefaultFramePtr = ListFramePtr_getActiveFramePtr(*DefaultFrameList);
    }while (DefaultFramePtr != NULL);
    return deletedSize;
}