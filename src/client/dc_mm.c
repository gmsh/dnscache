/*
 * filename:	dc_mm.c
 * author:	wakemecn
 * data:	Jun 28th, 2011
 * decription:	dnscache's memory management
 * 		but this is a fake version used by client
 *		to generate datrie tail pool where we can
 *		get random test set.
 */

#include "slist.h"
#include "dc_mm.h"


void * dc_alloc(size_t size)
{
	return  malloc(size);
}

void dc_free(void * ptr)
{
	return free(ptr);
}

void * dc_realloc(void * ptr, size_t size)
{
	return realloc(ptr, size);
}
