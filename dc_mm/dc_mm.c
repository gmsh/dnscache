/*
 * filename:	dc_mm.c
 * author:		wakemecn
 * data:		6/23/2011
 * decription:	dnscache's memory management
 */

#include "dc_mm.h"

void * dc_alloc(size_t size);

/*
 * call mm_pre_alloc() to pre-allocate number of counts' chunks with 
 * 2^capacity KB memory.
 * RETURN the pointer to a chunks list.
 */
struct slist * mm_pre_alloc(uint32 capacity, uint16 counts)
{
	struct slist * init_idle_list;
	void * chunk_ptr;
	//todo
	init_idle_list=mk_slist(malloc,counts);
	while(counts--){
		chunk_ptr=malloc(POW2(capacity));
		push(chunk_ptr,init_idle_list);
	}
	return init_idle_list;	
}

/*
 * the main thread calls mm_init() to initialize the momery managment 
 * modules.
 * RETURN 0 if succeed;
 * RETURN 1 if failed.
 */
int mm_init()
{
	int i = CHUNK_TYPE_NUM;
	while(i--){
		idle_chunks_table[i] = mm_pre_alloc((SMALL+i), num_each_chunks[i]);	
		chunks_manager_table[i] = (struct chunks_manager *)malloc(sizeof(struct chunks_manager));
		chunks_manager_table[i]->chunks_cap = SMALL+i;
		chunks_manager_table[i]->idle_num = num_each_chunks[i];
		chunks_manager_table[i]->idle_chunks = idle_chunks_table[i];
		chunks_manager_table[i]->alloced_chunks = NULL;
//		chunks_manager_table[i]->extra_chunks=NULL;
	}
	return 0;	
}

/* select a node from the pre-allocated idle list */
void * select_pre_alloced(uint32 cap)
{
	int pos=cap-SMALL;  /* position of the pointer in the array. */
	void * ptr;
	if(chunks_manager_table[pos]==0)
		return dc_alloc(POW2(cap));
	ptr=pop(chunks_manager_table[pos]->idle_chunks);
	push(ptr,chunks_manager_table[pos]->alloced_chunks);
	--(chunks_manager_table[pos]->idle_num);
	return ptr; 
} 

/* select a node frome the extra idle list */
void * select_extra(uint32 cap)
{
	void * ptr;
	if(elm_table[cap]->idle_num==0)
		return dc_alloc(POW2(cap));	
	ptr=pop(elm_table[cap]->chunks_list);
	append(ptr,elm_table[cap]->chunks_list);
	--(elm_table[cap]->idle_num);
	return ptr;
}

void * dc_alloc(size_t size)
{
	uint32 cap=floor((uint32)LOG2(size)); /* */
	struct slist * list_ptr;
	struct sl_node * node_ptr;
	int position=cap-SMALL;  /* position of the pointer in the array. */
	/* if capacity chunks not pre-allocated. */
	if( cap < SMALL || cap>BIG){ 
		/* if hasn't extra list */
		if(elm_table[cap]==NULL){ 
			elm_table[cap]=(struct extra_list_manager *)malloc(sizeof(struct extra_list_manager));
			list_ptr = mm_pre_alloc(cap , DEFAULT_EXTRA);
			elm_table[cap]->idle_num=DEFAULT_EXTRA;
			elm_table[cap]->total_num=DEFAULT_EXTRA;
			elm_table[cap]->chunks_list=list_ptr;
			return select_extra(cap);
		}
		/* if has extra list but full,then expand */
		if(elm_table[cap]->idle_num==0){
			list_ptr=elm_table[cap]->chunks_list;
			list_ptr=sl_expand(list_ptr,malloc,DEFAULT_EXTRA);
			node_ptr = list_ptr->blank;
			while(node_ptr++ < list_ptr->end)
					node_ptr->data=malloc(POW2(cap));
			elm_table[cap]->idle_num += DEFAULT_EXTRA;
			elm_table[cap]->total_num += DEFAULT_EXTRA;
		}
		return select_extra(cap);
	}
	/* capacity chunks pre-allocated .*/
	/* if no idle chunks */
	if(chunks_manager_table[position]->idle_num==0){
		/* if hasn't extra list ,then create it */
		if(elm_table[cap]==NULL){
			elm_table[cap]=(struct extra_list_manager *)malloc(sizeof(struct extra_list_manager));
			list_ptr=mm_pre_alloc(cap,DEFAULT_EXTRA);
            elm_table[cap]->idle_num=DEFAULT_EXTRA;
			elm_table[cap]->total_num += DEFAULT_EXTRA;
            elm_table[cap]->chunks_list=list_ptr;
            return select_extra(cap);
        } 
		/* if has extra list ,but full ,then expand it */
		if(elm_table[cap]->idle_num==0){
			list_ptr=elm_table[cap]->chunks_list;
			list_ptr=sl_expand(list_ptr,malloc,DEFAULT_EXTRA);
			node_ptr = list_ptr->blank;
			while(node_ptr++ < list_ptr->end)
				node_ptr->data=malloc(POW2(cap));
		 	elm_table[cap]->idle_num += DEFAULT_EXTRA;
			elm_table[cap]->total_num += DEFAULT_EXTRA;
		}  
		return select_extra(cap);
	}
	/* if has idle chunck */
	return select_pre_alloced(cap);
}

void dc_free(void * ptr){
	uint32 cap=LOG2(sizeof(ptr));
	int pos = cap - SMALL;
	struct sl_node * sn_ptr;
	struct slist * sl_ptr;
	/* if ptr is NULL,no operation is performed. */
	if(!ptr)
		return;
	/* if chunks wasn't pre-allocated */
	if(cap<SMALL || cap>BIG){
		sl_ptr = elm_table[cap]->chunks_list;
		mv2head(ptr,sl_ptr);
		if(++(elm_table[cap]->idle_num) == elm_table[cap]->total_num ){
			sl_free(free,elm_table[cap]->chunks_list);		
			free(elm_table[cap]);	
		}
		return;				
	}
	/* if chunks was pre-allocated */
	/* if chunk to be freed in the pre-alloc-list */
	sn_ptr = find(ptr,(sl_ptr = chunks_manager_table[pos]->alloced_chunks));
	if(sn_ptr){
		push(pop(sl_ptr),chunks_manager_table[pos]->idle_chunks);
		++(chunks_manager_table[pos]->idle_num);
	}		
	/* if chunk to be freed in the extra-list */
	sl_ptr = elm_table[cap]->chunks_list;
	mv2head(ptr,sl_ptr);
	(elm_table[cap]->idle_num)++;
	if(++(elm_table[cap]->idle_num) == elm_table[cap]->total_num ){
		sl_free(free,elm_table[cap]->chunks_list);		
		free(elm_table[cap]);	
	}
}

void * dc_realloc(void * ptr, size_t size)
{
	void * new_ptr;	
	if(size == 0)
		dc_free(ptr);
	if(size <= sizeof(ptr))
		return ptr;
	new_ptr = dc_alloc(POW2(floor(LOG2(size))));
	memcpy(new_ptr,ptr,sizeof(ptr));
	return new_ptr;
}
