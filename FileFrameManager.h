#ifndef FILEFRAMEMANAGER_H
#define FILEFRAMEMANAGER_H

#include <stdio.h>
#include "ID3v2Frames.h"
#include "ListFramePtr.h"
#include "ID3v2.h"


void FileManager_writteTXTFramesInFile(FILE *, ListFramePtr *);

void FileManager_writteCOMMFramesInFile(FILE *, ListFramePtr *);

void FileManager_writtePRIVFramesInFile(FILE *, ListFramePtr *);

void FileManager_writteMCDIFrameInFile(FILE *, ID3v2MCDIFrameType );

void FileManager_writtePOPMFrameInFile(FILE *, ID3v2POPMFrameType ); //REMOVE

void FileManager_writtePOPMFramesInFile(FILE *,ListFramePtr *);

void FileManager_writteAPICFrameInFile(FILE *, ID3v2APICFrameType );

void FileManager_writteAPICFramesInFile(FILE *, ListFramePtr * );

void FileManager_writtePadding(FILE *, int );

void FileManager_removeTagFromFile(char*);

void FileManager_writteTagIntoFile(char *, ID3TagType *);

#endif // FILEFRAMEMANAGER_H
