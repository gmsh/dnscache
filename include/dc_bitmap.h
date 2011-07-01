/*
 * bitmap using uint64
 * but can store 38 bit only
 * author gmsh
 * created on July 1,2011
 */

#ifndef _H_DC_BITMAP
#define _H_DC_BITMAP

#include "typedefs.h"

typedef uint64 _dc_bitmap;

inline _dc_bitmap bm_set(int8 i, _dc_bitmap  bm);

inline int8 bm_get(int8 i, _dc_bitmap bm);

/*return first 1, if bm == 0 return -1*/
inline int8 first_of_1(_dc_bitmap bm);

/* return number of 1*/
inline int8 num_of_1(_dc_bitmap bm);

/* return next 1. if touch end return -1 */
inline int8 next_of_1(int8 cur, _dc_bitmap bm);

#endif
