#include "dc_bitmap.h"

#define MASK 0x8000000000000000

inline _dc_bitmap bm_set(int8 i, _dc_bitmap  bm)
{
  return bm | (MASK >> (i - 1));
}

inline int8 bm_get(int8 i, _dc_bitmap bm)
{
  return bm & (MASK >> (i - 1));
}

/*return first 1, if bm == 0 return -1*/
inline int8 first_of_1(_dc_bitmap bm)
{
  uint8 i;
  for(i = 1; i <= 38; i++){
    if(bm_get(i, bm))
      return i;
  }
  return -1;
}

/* return number of 1*/
inline int8 num_of_1(_dc_bitmap bm)
{
  uint8 i, count = 0;
  for(i = 1; i <= 38; i++){
    if(bm_get(i, bm))
      count++;
  }
  return count;
}

/* return next 1. if touch end return -1 */
inline int8 next_of_1(int8 cur, _dc_bitmap bm)
{
  uint8 i;
  for(i = cur + 1; i <=38; i++){
    if(bm_get(i, bm))
      return i;
  }
  return -1;
}
