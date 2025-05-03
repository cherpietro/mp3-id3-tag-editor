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
    ListFramePtr APICFrameList; 
    //Not supported addition
    ListFramePtr PRIVFrameList;
    //Pending to include addition
    ListFramePtr WWWFrameList; 
    ListFramePtr COMMFrameList;
    ListFramePtr WXXXFrameList; 

    /*NotTested*/
    ID3v2MCDIFrameType *MCDI;
    ListFramePtr POPMFrameList; //LIST
    // ID3v2IPLSFrameType *IPLS;
    // ID3v2SYTCFrameType *SYTC;
    // ID3v2USERFrameType *USER;
    // ID3v2OWNEFrameType *OWNE;
    // ID3v2PCNTFrameType *PCNT;
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

void ID3v2_storeTagInStruct(char*, ID3TagType*);

bool ID3v2_storeNextFrameInStruct(FILE*, ID3TagType*);

void ID3v2_getTagSizeOfTheStruct(ID3TagType *);

void ID3v2_deleteFrame(ID3TagType *, char *);

void ID3v2_addFrame(ID3TagType *, char *);

#endif // ID3V2_H
