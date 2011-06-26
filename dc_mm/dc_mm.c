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
		chunk_manager_table[i]=(struct chunk_manager *)malloc(sizeof(struct chunk_manager));
		chunk_manager_table[i]->chunk_cap=SMALL+i;
		chunk_manager_table[i]->idle_num=num_each_chunks[i];
		chunk_manager_table[i]->idle_chunks=idle_chunks_table[i];
		chunk_manager_table[i]->alloced_chunks=null;
//		chunk_manager_table[i]->extra_chunks=null;
	}
	return 0;	
}

void * select 

void * dc_alloc(size_t size){
	uint32 cap=i(uint32)LOG2(size)+1; /* */
	struct slist * list_ptr;
	struct sl_node * node_ptr;
	int position=cap-SMALL;  /* position of the pointer in the array. */
	/* if capacity chunks not pre-allocated. */
	if( cap < SMALL || cap>BIG){
		/* if hasn't extra list */
		if(elm_table[cap]==null){
			elm_table[cap]=(struct extra_list_manager *)malloc(sizeof(struct extra_list_manager));
			list_ptr=mm_pre_alloc(cap,DEFAULT_EXTRA);
			elm_table[cap]->idle_num=DEFAULT_EXTRA;
			elm_table[cap]->chunks_list=list_ptr;
			return select_extra(elm_table[cap]);
		}
		/* if has extra list but full,then expand */
		if(elm_table[cap]->idle_num==0){
			list_ptr=elm_table[cap]->chunks_list;
			list_ptr=sl_expand(list_ptr,malloc(0),DEFAULT_EXTRA);
			while((node_ptr=list_ptr->blank)++ < list_ptr->end)
					node_ptr->data=malloc(POW2(cap));
			list_ptr->idle_num  += DEFAULT_EXTRA;
		}
		return select_extra(elm_table[cap]);
	}
	/* capacity chunks pre-allocated .*/i
	/* if no idle chunks */
	if(chunk_manager_table[position]->idle_num==0){
		/* if hasn't extra list ,then create it */
		if(elm_table[cap]==null){
			elm_table[cap]=(struct extra_list_manager *)malloc(sizeof(struct extra_list_manager));
			list_ptr=mm_pre_alloc(cap,DEFAULT_EXTRA);
            elm_table[cap]->idle_num=DEFAULT_EXTRA;
            elm_table[cap]->chunks_list=list_ptr;
            return select_extra(elm_table[cap]);
        } 
		/* if has extra list ,but full ,then expand it */
		if(elm_table[cap]->idle_num==0){
			list_ptr=elm_table[cap]->chunks_list;
			list_ptr=sl_expand(list_ptr,malloc(0),DEFAULT_EXTRA);
			while((node_ptr=list_ptr->blank)++ < list_ptr->end)
				node_ptr->data=malloc(POW2(cap));
		 	list_ptr->idle_num += DEFAULT_EXTRA;
		}  
		return select_extra(elm_table[cap]);
	}
	/* if has idle chunck */
	return select_pre_alloced(cap);
}

void dc_free(void * ptr)
{
  free(ptr);
}
