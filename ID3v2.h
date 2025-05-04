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
    ListFramePtr TXXXFrameList; 
    ListFramePtr TXTFrameList;
    ListFramePtr APICFrameList; 
    ListFramePtr WWWFrameList; 
    ListFramePtr WXXXFrameList; 
    ListFramePtr COMMFrameList;
    ListFramePtr POPMFrameList; 
    ID3v2PCNTFrameType *PCNT; //test
    //Not supported addition
    ListFramePtr PRIVFrameList;
    ID3v2DefaultFrameType *MCDI;
    //Pending to include addition
    /**/
    
    /*TO IMPLEMENT*/
    ListFramePtr UFIDFrameList; //LIST
    ListFramePtr USLTFrameList; //LIST
    ListFramePtr SYLTFrameList; //LIST
    ListFramePtr GEOBFrameList; //LIST
    ListFramePtr LINKFrameList; //List
    ListFramePtr AENCFrameList; //LIST
    ListFramePtr ENCRFrameList; //LIST
    ListFramePtr GRIDFrameList; //LIST
    
    // ID3v2DefaultFrameType *IPLS; 
    ID3v2IPLSFrameType *IPLS;
    ID3v2DefaultFrameType *SYTC; 
    ID3v2DefaultFrameType *USER; 
    ID3v2DefaultFrameType *OWNE; 
    //
    ID3v2DefaultFrameType *COMR; //???
    ID3v2DefaultFrameType *POSS;
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

int ID3v2_storeTagInStruct(char*, ID3TagType*);

bool ID3v2_getTagSizeOfTheStruct(ID3TagType *);

void ID3v2_deleteFrame(ID3TagType *, char *);

void ID3v2_addFrame(ID3TagType *, char *);


#endif // ID3V2_H
