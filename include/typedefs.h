/*
 * typedefs.h
 *
 *  Created on: Jun 25, 2011
 *      Author: gmsh
 */

#ifndef TYPEDEFS_H_
#define TYPEDEFS_H_
#include <limits.h>
/*
 * TODO according limits.h to define the below
 */
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long uint64;

typedef char int8;
typedef short int16;
typedef int int32;
typedef long int64;

#define likely(x)       __builtin_expect((x),1)
#define unlikely(x)     __builtin_expect((x),0)


#endif /* TYPEDEFS_H_ */
