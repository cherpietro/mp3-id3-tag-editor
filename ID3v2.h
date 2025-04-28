#ifndef ID3V2_H
#define ID3V2_H

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "ID3v2Header.h" 
#include "ID3v2Frames.h"
#include "TXTFrameList.h"
#include "COMMFrameList.h"
#include "PRIVFrameList.h"
#include "stdbool.h"

typedef struct {
    ID3v2HeaderType header;
    TXTFrameList TXTFrameList;
    COMMFrameList COMMFrameList;
    PRIVFrameList PRIVFrameList;
    ID3v2MCDIFrameType *MCDI;
    ID3v2POPMFrameType *POPM; //LIST
    ID3v2APICFrameType *APIC; //LIST
    /*Only stored*/
    ID3v2IPLSFrameType *IPLS;
    /**/
    /*TO IMPLEMENT*/
    int UFID; //LIST
    int LISTWXXX; //LIST
    int ETCO;
    int MLLT;
    int SYTC;
    int USLT; //LIST
    int SYLT; //LIST
    int RVAD;
    int EQUA;
    int RVRB;
    int GEOB; //LIST
    int PCNT;
    int RBUF;
    int AENC;
    int LINK;
    int POSS;
    int USER;
    int OWNE;
    int COMR;
    int ENCR;
    int GRID;
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

void ID3v2_saveAPICImage(ID3TagType *);

void ID3v2_getTagSizeOfTheStruct(ID3TagType *);

#endif // ID3V2_H
