#include "ID3v2Header.h"

int readHeader(FILE* mp3FilePointer, ID3v2HeaderType* header) {
    if (fread(header, sizeof(ID3v2HeaderType), 1, mp3FilePointer) == 1) {
        return 1;
    }
    return -1;
}
