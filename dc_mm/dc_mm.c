/*
 * filename:	dc_mm.c
 * author:	wakemecn
 * data:	6/23/2011
 * decription:	dnscache's memory management
 */

#include "dc_mm.h"

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
	printf("dc_mm : %s\n","chunks list with specific capacity " + POW2(capacity) +" %ld has been initilized.\n");
	return init_idle_list;	
}

/*
 * the main thread calls mm_init() to initialize the momery managment 
 * modules.
 */
void mm_init()
{
	int i = CHUNK_TYPE_NUM;
	while(i--){
		idle_chunks_table[i] = mm_pre_alloc((SMALL+i), num_each_chunks[i]);	
		chunks_manager_table[i] = (struct chunks_manager *)malloc(sizeof(struct chunks_manager));
		chunks_manager_table[i]->chunks_cap = SMALL+i;
		chunks_manager_table[i]->idle_num = num_each_chunks[i];
		chunks_manager_table[i]->idle_chunks = idle_chunks_table[i];
		chunks_manager_table[i]->alloced_chunks = NULL;
		pre_alloc_mutex[i] = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
		pthread_mutex_init(pre_alloc_mutex[i] , NULL);
	}
	int j = BIGGEST_CAP + 1;
	while(--j)
		extra_mutex[j] = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
		pthread_mutex_init(extra_mutex[j] , NULL);
	printf("dc_mm : %s\n","dns cache memory management modules has been initilized.");
	return 0;	
}

/* select a node from the pre-allocated idle list */
void * select_pre_alloced(uint64 cap)
{
	int pos = cap - SMALL;  /* position of the pointer in the array. */
	void * ptr , * chunk_ptr;
	if(0 == chunks_manager_table[pos]->idle_num)
		return dc_alloc(POW2(cap));
	pthread_mutex_lock(pre_alloc_mutex[pos]);
	ptr = pop(chunks_manager_table[pos]->idle_chunks);
	push(ptr,chunks_manager_table[pos]->alloced_chunks);
	--(chunks_manager_table[pos]->idle_num);
	pthread_mutex_unlock(pre_alloc_mutex[pos]);
	chunk_ptr = (void *)((uint64 *)ptr + 1);
	printf("dc_mm : %s\n","select a specific capacity " + POW2(cap) + "form pre-allocated chunks list.");
	return chunk_ptr; 
} 

/* select a node frome the extra idle list */
void * select_extra(uint64 cap)
{
	void * ptr , * chunk_ptr;
	if(0 == elm_table[cap]->idle_num)
		return dc_alloc(POW2(cap));
	pthread_mutex_lock(extra_mutex[cap]);
	ptr = pop(elm_table[cap]->chunks_list);
	append(ptr,elm_table[cap]->chunks_list);
	--(elm_table[cap]->idle_num);
	pthread_mutex_unlock(extra_mutex[cap]);
	chunk_ptr = (void *)((uint64 *)ptr + 1);
	printf("dc_mm : %s\n","select a specific capacity " + POW2(cap) + "form extra chunks list.");
	return chunk_ptr;
}

/* make a new extra list with specific capacity */
void mk_el(uint64 cap){
	printf("dc_mm : %s\n.","start to generate extra chunks list of chunks with specific capacity " + POW2(cap) + ".");
	struct slist * list_ptr;
	if(!elm_table[cap])
		return;
	elm_table[cap] = (struct extra_list_manager *)malloc(sizeof(struct extra_list_manager));
	list_ptr = mm_pre_alloc(cap , DEFAULT_EXTRA);
	elm_table[cap]->idle_num = DEFAULT_EXTRA;
	elm_table[cap]->total_num = DEFAULT_EXTRA;
	elm_table[cap]->chunks_list = list_ptr;
	printf("dc_mm : %s\n","finish to generate extra chunks list of chunks with specific capacity " + POW2(cap) + ".");
}

/* expand a extra list with specific capacity */
void expand_el(uint64 cap)
{
	printf("dc_mm : %s\n.","start to expand extra chunks list of chunks with specific capacity " + POW2(cap) + ".");
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
	printf("dc_mm : %s\n.","finish to expand extra chunks list of chunks with specific capacity " + POW2(cap) +".");
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
			pthread_mutex_lock(extra_mutex[cap]);
			mk_el(cap);	
			pthread_mutex_unlock(extra_mutex[cap]);
			return select_extra(cap);
		}
		/* if has extra list but full,then expand */
		if(0 == elm_table[cap]->idle_num)
			pthread_mutex_lock(extra_mutex[cap]);
			expand_el(cap);	
			pthread_mutex_unlock(extra_mutex[cap]);
		return select_extra(cap);
	}
	/* capacity chunks pre-allocated .*/
	/* if no idle chunks */
	if(0 == chunks_manager_table[position]->idle_num){
		/* if hasn't extra list ,then create it */
		if(NULL == elm_table[cap]){
			pthread_mutex_lock(extra_mutex[cap]);
			mk_el(cap);            
			pthread_mutex_unlock(extra_mutex[cap]);
			return select_extra(cap);
        } 
		/* if has extra list ,but full ,then expand it */
		if(0 == elm_table[cap]->idle_num)
			pthread_mutex_lock(extra_mutex[cap]);
			expand_el(cap);
			pthread_mutex_unlock(extra_mutex[cap]);
		return select_extra(cap);
	}
	/* if has idle chunck */
	return select_pre_alloced(cap);
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
		pthread_mutex_lock(extra_mutex[cap]);
		mv2head(word_before_ptr,sl_ptr);
		printf("dc_mm : %s","free a extra chunk with specific capacity " + POW2(cap) + ".");
		if(++(elm_table[cap]->idle_num) == elm_table[cap]->total_num ){
			sl_free(free,elm_table[cap]->chunks_list);		
			free(elm_table[cap]);	
			printf("dc_mm : %s","free a extra chunks list  with specific capacity " + POW2(cap) + ".");
		}
		pthread_mutex_unlock(extra_mutex[cap]);
		return;				
	}
	/* if chunks was pre-allocated */
	/* if chunk to be freed in the pre-alloc-list */
	sl_ptr = chunks_manager_table[pos]->alloced_chunks;
	int found = find(word_before_ptr,sl_ptr);
	if(found){
		printf("dc_mm : %s","free a chunk with specific capacity " + POW2(cap) + ".");
		pthread_mutex_lock(pre_alloc_mutex[pos]);
		push(pop(sl_ptr),chunks_manager_table[pos]->idle_chunks);
		++(chunks_manager_table[pos]->idle_num);
		pthread_mutex_unlock(pre_alloc_mutex[pos]);
	}		
	/* if chunk to be freed in the extra-list */
	sl_ptr = elm_table[cap]->chunks_list;
	pthread_mutex_lock(extra_mutex[cap]);
	mv2head(word_before_ptr,sl_ptr);  
	printf("dc_mm : %s","free a extra chunk with specific capacity " + POW2(cap) + ".");
	(elm_table[cap]->idle_num)++;
	if(elm_table[cap]->idle_num == elm_table[cap]->total_num ){
		sl_free(free,elm_table[cap]->chunks_list);		
		free(elm_table[cap]);	
		printf("dc_mm : %s","free a extra chunks list  with specific capacity " + POW2(cap) + ".");
	}
	pthread_mutex_unlock(extra_mutex[cap]);
}

void * dc_realloc(void * ptr, size_t size)
{
	void * word_before_ptr = (void *)((uint64 *)ptr - 1);
	void * new_ptr;
	uint64 cap = *(uint64 *)word_before_ptr;
	if(size == 0)
		dc_free(ptr);
	if(size <= POW2(cap)){
		printf("dc_mm : %s","reallocate but return the original pointer.");
		return ptr;
	}
	new_ptr = dc_alloc(size);
	memcpy(new_ptr,ptr,POW2(cap));
	dc_free(ptr);
	printf("dc_mm : %s","reallocation finished");
	return new_ptr;
}
