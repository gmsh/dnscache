/*
 * filename:	dc_mm.c
 * author:		wakemecn
 * data:		6/23/2011
 * decription:	dnscache's memory management
 */

#include <dc_mm.h>
#include <mm_structs.h>

int mm_init(){
	
	
	
}

void * dc_alloc(size_t size)
{
  return malloc(size);
}

void dc_free(void * ptr)
{
  free(ptr);
}
#endif
