/*
 * filename:	dc_mm.c
 * author:	wakemecn
 * data:	6/23/2011
 * decription:	dnscache's memory management
 */

#include "dc_mm_structs.h"

void * dc_alloc(size_t size);

/*
 * call mm_pre_alloc() to pre-allocate number of counts' chunks with 
 * 2^capacity KB memory.
 * RETURN the pointer to a chunks list.
 */
struct slist * mm_pre_alloc(uint64 capacity, uint16 counts)
{
	struct slist * init_idle_list;
	void * ptr , * chunk_ptr;
	init_idle_list = mk_slist(malloc,counts);  /* 构造链表 */
	while(counts--){
		ptr = malloc(POW2(capacity)+8);
		*(uint64 *)ptr = capacity;
		chunk_ptr = (void *)((uint64 *)ptr + 1);
		push(ptr,init_idle_list);  /* 压入链表中*/
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
	}
	return 0;	
}

/* select a node from the pre-allocated idle list */
void * select_pre_alloced(uint64 cap)
{
	int pos = cap - SMALL;  /* position of the pointer in the array. */
	void * ptr , * chunk_ptr;
	if(0 == chunks_manager_table[pos]->idle_num)
		return dc_alloc(POW2(cap));
	ptr = pop(chunks_manager_table[pos]->idle_chunks);
	push(ptr,chunks_manager_table[pos]->alloced_chunks);
	--(chunks_manager_table[pos]->idle_num);
	chunk_ptr = (void *)((uint64 *)ptr + 1);
	return chunk_ptr; 
} 

/* select a node frome the extra idle list */
void * select_extra(uint64 cap)
{
	void * ptr , * chunk_ptr;
	if(0 == elm_table[cap]->idle_num)
		return dc_alloc(POW2(cap));
	ptr = pop(elm_table[cap]->chunks_list);
	append(ptr,elm_table[cap]->chunks_list);
	--(elm_table[cap]->idle_num);
	chunk_ptr = (void *)((uint64 *)ptr + 1);
	return chunk_ptr;
}

/* make a new extra list with specific capacity */
void mk_el(uint64 cap){
	struct slist * list_ptr;
	elm_table[cap] = (struct extra_list_manager *)malloc(sizeof(struct extra_list_manager));
	list_ptr = mm_pre_alloc(cap , DEFAULT_EXTRA);
	elm_table[cap]->idle_num = DEFAULT_EXTRA;
	elm_table[cap]->total_num = DEFAULT_EXTRA;
	elm_table[cap]->chunks_list = list_ptr;
}

/* expand a extra list with specific capacity */
void expand_el(uint64 cap)
{
	struct slist * list_ptr;
	void * ptr , * chunk_ptr;
	list_ptr = elm_table[cap]->chunks_list;
	list_ptr = sl_expand(list_ptr,malloc,DEFAULT_EXTRA);
	int i = DEFAULT_EXTRA;	
	while(i--){
		*(uint64 *)ptr = cap;
		void * chunk_ptr = (void *)((uint64 *)ptr + 1);
		push(ptr , list_ptr);
	}
	elm_table[cap]->idle_num += DEFAULT_EXTRA;
	elm_table[cap]->total_num += DEFAULT_EXTRA;
}

void * dc_alloc(size_t size)
{
	uint64 cap = (uint64)LOG2((uint64)size); /* */
	if((uint64)size > (0x000000000000001 << cap))
		cap++;
	struct slist * list_ptr;
	struct sl_node * node_ptr;
	int position = cap-SMALL;  /* position of the pointer in the array. */
	/* if capacity chunks not pre-allocated. */
	if( cap < SMALL || cap > BIG){ 
		/* if hasn't extra list */
		if(NULL == elm_table[cap]){
			mk_el(cap);	
			return select_extra(cap);
		}
		/* if has extra list but full,then expand */
		if(0 == elm_table[cap]->idle_num)
			expand_el(cap);	
		return select_extra(cap);
	}
	/* capacity chunks pre-allocated .*/
	/* if no idle chunks */
	if(0 == chunks_manager_table[position]->idle_num){
		/* if hasn't extra list ,then create it */
		if(NULL == elm_table[cap]){
			mk_el(cap);            
			return select_extra(cap);
        } 
		/* if has extra list ,but full ,then expand it */
		if(0 == elm_table[cap]->idle_num)
			expand_el(cap);
		return select_extra(cap);
	}
	/* if has idle chunck */
	return select_pre_alloced(cap);
}

void free_extra(uint64 cap){
	struct slist * sl_ptr;
	sl_ptr = elm_table[cap]->chunks_list;
	
}

void dc_free(void * ptr){
	void * word_before_ptr = (uint64 *)ptr - 1;
	uint64 cap = *(uint64 *)word_before_ptr;
	int pos = cap - SMALL;
	struct sl_node * sn_ptr;
	struct slist * sl_ptr;
	/* if ptr is NULL,no operation is performed. */
	if(!ptr)
		return;
	/* if chunks wasn't pre-allocated */
	if(cap < SMALL || cap > BIG){
		sl_ptr = elm_table[cap]->chunks_list;
		mv2head(word_before_ptr,sl_ptr);
		if(++(elm_table[cap]->idle_num) == elm_table[cap]->total_num ){
			sl_free(free,elm_table[cap]->chunks_list);		
			free(elm_table[cap]);	
		}
		return;				
	}
	/* if chunks was pre-allocated */
	/* if chunk to be freed in the pre-alloc-list */
	sl_ptr = chunks_manager_table[pos]->alloced_chunks;
	int found = find(word_before_ptr,sl_ptr);
	if(found){
		push(pop(sl_ptr),chunks_manager_table[pos]->idle_chunks);
		++(chunks_manager_table[pos]->idle_num);
	}		
	/* if chunk to be freed in the extra-list */
	sl_ptr = elm_table[cap]->chunks_list;
	mv2head(word_before_ptr,sl_ptr);
	(elm_table[cap]->idle_num)++;
	if(elm_table[cap]->idle_num == elm_table[cap]->total_num ){
		sl_free(free,elm_table[cap]->chunks_list);		
		free(elm_table[cap]);	
	}
}

void * dc_realloc(void * ptr, size_t size)
{
	void * word_before_ptr = (void *)((uint64 *)ptr - 1);
	void * new_ptr;
	uint64 cap = *(uint64 *)word_before_ptr;
	if(size == 0)
		dc_free(ptr);
	if(size <= POW2(cap))
		return ptr;
	new_ptr = dc_alloc(size);
	memcpy(new_ptr,ptr,POW2(cap));
	return new_ptr;
}
