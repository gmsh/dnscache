#ifndef _DC_UTILS_H
#define DC_UTILS_H

#define _DC_DEBUG

#include <string.h>

inline int dc_strlen(uint8 * s)
{
  int n;
  for(n = 0; *s != '\0'; s++){
    n++;
  }
  return n;
}

#endif
