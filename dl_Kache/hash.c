#include "hash.h"

uint32 BKDRHash(uint8 *str)
{
  uint32 hash = 0;
 
  while (*str)
    {
      hash = hash * 131 + (*str++);
    }
 
  return (hash & 0x1ffffff);
}
