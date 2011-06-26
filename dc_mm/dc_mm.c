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
	//todo
	init_idle_list=mk_slist(malloc(0),counts);
	while(counts--){
		chunk_ptr=malloc(POW2(capacity));
		push(chunk_ptr,init_idle_list);
	}
	return init_idle_list;	
}

int mm_init(){
	uint32 i;
	while((i=CHUNK_TYPE_NUM)--){
		idle_chunks_table[i]=mm_pre_alloc((SMALL+i), num_each_chunks[i]);	
		chunk_manager_table[i]=(struct chunk_manager *)malloc(sizeof(chunk_manager));
		chunk_manager_table[i]->chunk_cap=SMALL+i;
		chunk_manager_table[i]->idle_num=num_each_chunks[i];
		chunk_manager_table[i]->idle_chunks=idle_chunks_table[i];
		chunk_manager_table[i]->alloced_chunks=null;
		chunk_manager_table[i]->extra_chunks=null;
	}
	return 0;	
}

void * dc_alloc(size_t size){
	uint32 cap=i(uint32)LOG2(size)+1; /* */
	struct slist * extra_list;
	int position=cap-SMALL;  /* position of the pointer in the array. */
	if( cap < SMALL || cap>BIG){
		if(elm_table[cap]==null){
			elm_table[cap]=chunks_list=mm_pre_alloc(cap,DEFAULT_EXTRA);
			return select_extra(cap,elm_table[cap]);
		}
		if(elm_table[cap]->idle_num==0)
			elm_table[cap]->chunks_list=sl_expand(elm_table[cap]->chunks_list,malloc(0),DEFAULT_EXTRA);
		return select_extra(cap,elm_table[cap]);

	}



		|| chunk_manager_table[position]->idle_num==0){
		extra_list=mm_pre_alloc(cap,num_extra_chunks[position]);
		chunk_manager_table[positon]->extra_chunks=extra_list;
	}
	if()
	return ;
}

void dc_free(void * ptr)
{
  free(ptr);
}
