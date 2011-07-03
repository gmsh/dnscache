/*
 * filename:	dc_mm.c
 * author:	wakemecn
 * data:	6/23/2011
 * decription:	dnscache's memory management
 */

#define	LOG2(x) ffs(x)
#define POW2(x) (0x0000000000000001 << x)

#include "slist.h"
#include "dc_mm.h"
#include "dc_mm_structs.h"

void * dc_alloc(size_t size)
{
	return  malloc(size);
}

void dc_free(void * ptr)
{
	return free(ptr);
}

void * dc_realloc(void * ptr, size_t size)
{}
