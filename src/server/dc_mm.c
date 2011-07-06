/*
 * filename:	dc_mm.c
 * author:	wakemecn
 * data:	6/23/2011
 * decription:	dnscache's memory management
 */

#include "typedefs.h"
#include "dc_mm_structs.h"
#include "dc_mm.h"

void * dc_alloc(size_t size)
{
	return malloc(size);
}
void dc_free(void * ptr){
	return free(ptr);
}
void * dc_realloc(void * ptr, size_t size)
{
	return realloc(ptr, size);
}
void mm_init(){}
