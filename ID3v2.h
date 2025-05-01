#ifndef ID3V2_H
#define ID3V2_H

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "ID3v2Header.h" 
#include "ID3v2Frames.h"
#include "ListFramePtr.h"
#include "stdbool.h"

typedef struct {
    ID3v2HeaderType header;
    ListFramePtr TXTFrameList;
    // TXTFrameList TXTFrameList;
    ListFramePtr COMMFrameList;
    // COMMFrameList COMMFrameList;
    ListFramePtr PRIVFrameList;
    // PRIVFrameList PRIVFrameList;
    ListFramePtr APICFrameList; 
    // ID3v2APICFrameType *APIC; //LIST
    ID3v2MCDIFrameType *MCDI;
    ID3v2POPMFrameType *POPM; //LIST
    /*Only stored*/
    ID3v2IPLSFrameType *IPLS;
    ID3v2SYTCFrameType *SYTC;
    ID3v2USERFrameType *USER;
    ID3v2OWNEFrameType *OWNE;
    ID3v2PCNTFrameType *PCNT;
    /**/
    /*TO IMPLEMENT*/
    int UFID; //LIST
    int LISTWXXX; //LIST
    int USLT; //LIST
    int SYLT; //LIST
    int GEOB; //LIST
    int LINK; //List
    
   
    int AENC;
    int POSS;
    int COMR;
    int ENCR;
    int GRID;

    ID3v2DefaultFrameType *RVRB;
    ID3v2DefaultFrameType *EQUA;
    ID3v2DefaultFrameType *MLLT;
    ID3v2DefaultFrameType *ETCO;
    ID3v2DefaultFrameType *RVAD;
    /**/
    size_t paddingSize;
} ID3TagType;

void ID3v2_init(ID3TagType *);

void ID3v2_free(ID3TagType *);

void ID3v2_storeTagInStruct(char*, ID3TagType*);

bool ID3v2_storeNextFrameInStruct(FILE*, ID3TagType*);

void ID3v2_removeTagFromFile(char*);

void ID3v2_writteTagIntoFile(char *, ID3TagType *);

void printTag(ID3TagType *);

void ID3v2_getTagSizeOfTheStruct(ID3TagType *);

void ID3v2_listFrames(ID3TagType *);

void ID3v2_printFrame(ID3TagType *, char *);

void ID3v2_deleteFrame(ID3TagType *, char *);

void ID3v2_addFrame(ID3TagType *, char *);

#endif // ID3V2_H
