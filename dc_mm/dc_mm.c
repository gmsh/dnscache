/*
 * filename:	dc_mm.c
 * author:		wakemecn
 * data:		6/23/2011
 * decription:	dnscache's memory management
 */

#include "dc_mm.h"
#include "mm_structs.h"

struct slist * mm_pre_alloc(uint32 capacity, uint16 counts){
	struct slist * init_idle_list;
	void * chunck_ptr;
	while(counts--){
		chunk_ptr=malloc(pow(2,capacity));
		push(chunk_ptr,init_idle_list);
	}
	return init_idle_list;	
}

int mm_init(){
	uint32 i;
	while((i=CHUNK_TYPE_NUM)--){
		idle_chunks[i]=mm_pre_alloc((SMALL+i), num_each_chunks[i])	
	}
	return 0;	
}

void * dc_alloc(size_t size)
{
  return malloc(size);
}

void dc_free(void * ptr)
{
  free(ptr);
}
