#include "SizeReader.h"

uint32_t syncsafeToSize( uint8_t *size) {
  return (size[0] << 21) | (size[1] << 14) | (size[2] << 7) | size[3];
}

uint32_t sizeFromID3v23(uint8_t *size) {
  return (size[0] << 24) | (size[1] << 16) | (size[2] << 8) | size[3];
}