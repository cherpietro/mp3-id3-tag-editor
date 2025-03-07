#include "ID3v2Header.h"

void readHeader(FILE* mp3FilePointer, ID3v2HeaderType* header) {
    fread(header, sizeof(ID3v2HeaderType), 1, mp3FilePointer);
}
