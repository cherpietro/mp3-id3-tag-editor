
#ifndef SIZEREADER_H
#define SIZEREADER_H

#include <stdint.h>

uint32_t syncsafeToSize(uint8_t*);

uint32_t sizeFromID3v23(uint8_t*);

#endif // SIZEREADER_H
