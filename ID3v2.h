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
    ListFramePtr PRIVFrameList; //Binary Data
    ID3v2DefaultFrameType *MCDI; //Binary Data
    //Pending to include addition
    /**/
    
    /*TO IMPLEMENT*/
    ListFramePtr UFIDFrameList; //Binary Data
    ListFramePtr USLTFrameList; //SI hay tiempo lo meto
    ListFramePtr SYLTFrameList; //Unkown format
    ListFramePtr GEOBFrameList; //Binary Data
    ListFramePtr LINKFrameList; //Unkown format
    ListFramePtr AENCFrameList; //Binary Data
    ListFramePtr ENCRFrameList; //Binary Data
    ListFramePtr GRIDFrameList; //Binary Data
    
    ID3v2IPLSFrameType *IPLS; //Done
    ID3v2DefaultFrameType *SYTC; //Binary Data
    ID3v2USERFrameType *USER; //Done
    ID3v2DefaultFrameType *OWNE; //date attribute unkown
    //
    ID3v2DefaultFrameType *COMR; //Binaty Data
    ID3v2DefaultFrameType *POSS; //Binaty Data
    ID3v2RVRBFrameType *RVRB; //Stored but not submited by user
    ID3v2DefaultFrameType *EQUA; //Binary Data
    ID3v2DefaultFrameType *MLLT; //Unkown format
    ID3v2DefaultFrameType *ETCO; //Unkown format
    ID3v2DefaultFrameType *RVAD; //Unkown format
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
