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
    int EQUA;
    int RVRB;
    int GEOB; //LIST
    int RBUF;
    int AENC;
    int LINK; //List
    int POSS;
    int COMR;
    int ENCR;
    int GRID;

    int MLLT;
    int ETCO;
    int RVAD;
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
