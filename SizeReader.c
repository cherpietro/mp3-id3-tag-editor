#include "SizeReader.h"

uint32_t syncsafeToSize( uint8_t *size) {
  return (size[0] << 21) | (size[1] << 14) | (size[2] << 7) | size[3];
}

uint32_t sizeFromID3v23(uint8_t *size) {
  return (size[0] << 24) | (size[1] << 16) | (size[2] << 8) | size[3];
}

void saveSizeToSyncsafe(uint32_t size, uint8_t *out) {
  out[0] = (size >> 21) & 0x7F;
  out[1] = (size >> 14) & 0x7F;
  out[2] = (size >> 7) & 0x7F;
  out[3] = size & 0x7F;
}

void saveSizeToID3v23(uint32_t size, uint8_t *out) {
  out[0] = (size >> 24) & 0xFF;
  out[1] = (size >> 16) & 0xFF;
  out[2] = (size >> 8) & 0xFF;
  out[3] = size & 0xFF;
}