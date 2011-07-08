/*
 * filename:	dc_mm.c
 * author:	wakemecn
 * data:	6/23/2011
 * decription:	dnscache's memory management
 */

#include "typedefs.h"
#include "dc_mm_structs.h"
#include "dc_mm.h"

/* 
 * ceil of log2(x) 
 */
#define LOG2(x) (first_bit(x) - 1) 
#define POW2(x) (0x0000000000000001 << x)
#define MASK 0x8000000000000000

/* number of each type of chunks */
static uint64 chunks_num[] = {	512, 512, 512, 512, 512,
				256, 256, 256, 256, 256,
				128, 128, 128, 128, 128,
				56, 56, 56, 56, 56
			     };

/*
 * call mm_pre_alloc() to pre-allocate number of counts'
 * chunks with 2^capacity KB memory.
 * RETURN the pointer to a chunks list.
 */
struct slist * mm_pre_alloc(uint64 capacity, uint16 counts)
{
	struct slist * init_idle_list;
	void * ptr;
	/* make a blank single list. */
	init_idle_list = mk_slist(malloc,counts);  
	while(counts--){
		/* malloc for each element of the list. */
		ptr = malloc(POW2(capacity)+sizeof(uint64));
		/* filled the capacity before the chunk. */
		*(uint64 *)ptr = capacity;	
		/* push the chunk into list. */
		push(ptr,init_idle_list); 
	}
	return init_idle_list;	
}

void mm_init()
{
	int i = CHUNK_TYPE_NUM;
	while(i--){
		/* init all the pre-allocated chunks */
		chunks_manager_table[i] = (struct chunks_manager *)malloc
					   (sizeof(struct chunks_manager));
		if(!chunks_manager_table[i]){
			printf("dc_mm : initilization failed!!!");	
			exit(1);
		}
		chunks_manager_table[i]->chunks_cap = SMALL+i;
		chunks_manager_table[i]->idle_num = chunks_num[i];
		chunks_manager_table[i]->idle_chunks = 
			mm_pre_alloc((SMALL+i), chunks_num[i]);	
		chunks_manager_table[i]->alloced_chunks = 
			mk_slist(malloc, chunks_num[i]);
/*
		printf("dc_mm : %s%8d%s\n","chunks list and extra chunks 
		       "list with specific capacity ",POW2(SMALL + i),
		       " Bytes has been initilized.");
*/
		pre_alloc_mutex[i] = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
		pthread_mutex_init(pre_alloc_mutex[i] , NULL);
		pre_alloc_apply[i] = pre_alloc_free[i] = 0;
	}
	int j = BIGGEST_CAP + 1;
	while(--j){
		extra_mutex[j] = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
		pthread_mutex_init(extra_mutex[j] , NULL);
		extra_apply[j] = extra_free[j] = 0;
	}
	printf("dc_mm : %s\n","dns cache memory management "
	       "modules has been initilized.");
	return;	
}

/* select a node from the pre-allocated idle list */
void * select_pre_alloced(uint64 cap)
{
	int pos = cap - SMALL;  /* position of the pointer in the array. */
	void * ptr , * chunk_ptr;
	pthread_mutex_lock(pre_alloc_mutex[pos]);
	if(0 == chunks_manager_table[pos]->idle_num){
		pthread_mutex_unlock(pre_alloc_mutex[pos]);
		return dc_alloc(POW2(cap));
	}
	ptr = pop(chunks_manager_table[pos]->idle_chunks);
	push(ptr,chunks_manager_table[pos]->alloced_chunks);
	--(chunks_manager_table[pos]->idle_num);
	pre_alloc_apply[pos]++;
	pthread_mutex_unlock(pre_alloc_mutex[pos]);
	chunk_ptr = (void *)((uint64 *)ptr + 1);
/*
	printf("dc_mm : %s%8d%s\n","select a specific capacity ", POW2(cap),
	       " Bytes form pre-allocated chunks list.");
*/	
	return chunk_ptr; 
} 

/* select a node frome the extra idle list */
void * select_extra(uint64 cap)
{
	void * ptr , * chunk_ptr;
	pthread_mutex_lock(extra_mutex[cap]);
	if(NULL == elm_table[cap]){
		pthread_mutex_unlock(extra_mutex[cap]);
		return dc_alloc(POW2(cap));
	}
	if(0 == elm_table[cap]->idle_num){
		pthread_mutex_unlock(extra_mutex[cap]);
		return dc_alloc(POW2(cap));
	}
	ptr = pop(elm_table[cap]->chunks_list);
	append(ptr,elm_table[cap]->chunks_list);
	--(elm_table[cap]->idle_num);
	extra_apply[cap]++;
	pthread_mutex_unlock(extra_mutex[cap]);
	chunk_ptr = (void *)((uint64 *)ptr + 1);
/*
	printf("dc_mm : %s%8d%s\n","select a specific capacity ", POW2(cap), 
	     " Bytes form extra chunks list.");
*/
	return chunk_ptr;
}

/* make a new extra list with specific capacity */
void mk_el(uint64 cap){
/*
	printf("dc_mm : %s%8d%s\n","start to generate extra "
		"chunks list of chunks with specific capacity ", 
	      POW2(cap), " Bytes.");
*/
	struct slist * list_ptr;
	if(elm_table[cap])
		return;
	elm_table[cap] = (struct extra_list_manager *)malloc(sizeof(struct extra_list_manager));
	list_ptr = mm_pre_alloc(cap , DEFAULT_EXTRA);
	elm_table[cap]->idle_num = DEFAULT_EXTRA;
	elm_table[cap]->total_num = DEFAULT_EXTRA;
	elm_table[cap]->chunks_list = list_ptr;
/*
	printf("dc_mm : %s%8d%s\n","finish to generate extra "
		"chunks list of chunks with specific capacity ",
	      POW2(cap), " Bytes.");
*/
}

/* expand a extra list with specific capacity */
void expand_el(uint64 cap)
{
/*
	printf("dc_mm : %s%8d%s\n","start to expand extra chunks list of "
		"chunks with specific capacity ", 
	        POW2(cap), " Bytes.");
*/
	struct slist * list_ptr;
	void * ptr , * chunk_ptr;
	list_ptr = elm_table[cap]->chunks_list;
	list_ptr = sl_expand(list_ptr,malloc,DEFAULT_EXTRA);
	int i = DEFAULT_EXTRA;	
	while(i--){
		ptr = malloc(sizeof(uint64) + POW2(cap));//added by wakemecn
		*(uint64 *)ptr = cap;
		void * chunk_ptr = (void *)((uint64 *)ptr + 1);
		push(ptr , list_ptr);
	}
	elm_table[cap]->idle_num += DEFAULT_EXTRA;
	elm_table[cap]->total_num += DEFAULT_EXTRA;
/*
	printf("dc_mm : %s%8d%s\n","finish to expand extra chunks list of chunks with specific capacity ", 
	        POW2(cap), " Bytes.");
*/
}

void * dc_alloc(size_t size)
{
	if(size == 0)
		return NULL;
	uint64 cap = (uint64)LOG2((uint64)size); /* */
	if((uint64)size > (POW2(cap)))
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
		if(0 == elm_table[cap]->idle_num){
			pthread_mutex_lock(extra_mutex[cap]);
			expand_el(cap);	
			pthread_mutex_unlock(extra_mutex[cap]);
		}
		return select_extra(cap);
	}
	/* capacity chunks pre-allocated .*/
	/* if no idle chunks */
	if(0 == chunks_manager_table[position]->idle_num){
		/* if hasn't extra list ,then create it */
		pthread_mutex_lock(extra_mutex[cap]);
		if(NULL == elm_table[cap]){
			mk_el(cap);            
			pthread_mutex_unlock(extra_mutex[cap]);
			return select_extra(cap);
        	} 
		/* if has extra list ,but full ,then expand it */
		if(0 == elm_table[cap]->idle_num){
			expand_el(cap);
		}
		pthread_mutex_unlock(extra_mutex[cap]);
		return select_extra(cap);
	}
	/* if has idle chunck */
	return select_pre_alloced(cap);
}

void free_extra_data(struct sl_node * sl_ptr)
{
	free(sl_ptr->data);	
}

void dc_free(void * ptr){
	/* if ptr is NULL,no operation is performed. */
	if(!ptr)
		return;
	void * word_before_ptr = (uint64 *)ptr - 1;
	uint64 cap = *(uint64 *)word_before_ptr;
	int pos = cap - SMALL;
	struct sl_node * sn_ptr;
	struct slist * sl_ptr;
	/* if chunks wasn't pre-allocated */
	if(cap < SMALL || cap > BIG){
		sl_ptr = elm_table[cap]->chunks_list;
		pthread_mutex_lock(extra_mutex[cap]);
		mv2head(word_before_ptr,sl_ptr);
/*
		printf("dc_mm : %s%8d%s\n","free a extra chunk with specific capacity ", 
		        POW2(cap), " Bytes.");
*/
		if(++(elm_table[cap]->idle_num) == elm_table[cap]->total_num ){
			traverse(free_extra_data,elm_table[cap]->chunks_list);
			sl_free(free,elm_table[cap]->chunks_list);		
			free(elm_table[cap]);	
			elm_table[cap] = NULL;	
/*
			printf("dc_mm : %s%8d%s\n","free a extra chunks list  with specific capacity ", 
			        POW2(cap), " Bytes.");
*/
		}
		extra_free[cap]++;
		pthread_mutex_unlock(extra_mutex[cap]);
		return;				
	}
	/* if chunks was pre-allocated */
	/* if chunk to be freed in the pre-alloc-list */
	sl_ptr = chunks_manager_table[pos]->alloced_chunks;
	int found = find(word_before_ptr,sl_ptr);
	if(found){
		pthread_mutex_lock(pre_alloc_mutex[pos]);
		mv2head(word_before_ptr,sl_ptr);
		push(pop(sl_ptr),chunks_manager_table[pos]->idle_chunks);
		++(chunks_manager_table[pos]->idle_num);
		pre_alloc_free[pos]++;
		pthread_mutex_unlock(pre_alloc_mutex[pos]);
/*
		printf("dc_mm : %s%8d%s\n","free a chunk with specific capacity ", 
		        POW2(cap), " Bytes.");
*/
		return; /* added by wakemecn at Jun 30th, 2011 */
	}		
	/* if chunk to be freed in the extra-list */
	pthread_mutex_lock(extra_mutex[cap]);
	if(!elm_table[cap]){
		pthread_mutex_unlock(extra_mutex[cap]);
		return;
	}
	sl_ptr = elm_table[cap]->chunks_list;
	mv2head(word_before_ptr,sl_ptr);  
/*
	printf("dc_mm : %s%8d%s\n","free a extra chunk with specific capacity ", 
	        POW2(cap), " Bytes.");
*/
	(elm_table[cap]->idle_num)++;
	if(elm_table[cap]->idle_num == elm_table[cap]->total_num ){
		traverse(free_extra_data,elm_table[cap]->chunks_list);
		sl_free(free,elm_table[cap]->chunks_list);		
		free(elm_table[cap]);
		elm_table[cap] = NULL;	
/*
		printf("dc_mm : %s%8d%s\n","free a extra chunks list  with specific capacity ", 
		        POW2(cap), " Bytes.");
*/
	}
	extra_free[cap]++;
	pthread_mutex_unlock(extra_mutex[cap]);
}

void * dc_realloc(void * ptr, size_t size)
{
	if(!ptr){
		ptr = dc_alloc(size);
		return ptr;
	}
	if(size == 0){
		dc_free(ptr);
		return NULL;
	}
	void * word_before_ptr = (void *)((uint64 *)ptr - 1);
	void * new_ptr;
	uint64 cap = *(uint64 *)word_before_ptr;
	if(size <= POW2(cap)){
/*
		printf("dc_mm : %s","reallocate but return the original pointer.");
		return ptr;
*/
	}
	new_ptr = dc_alloc(size);
	memcpy(new_ptr, ptr, POW2(cap));
	dc_free(ptr);
//	printf("dc_mm : %s","reallocation finished");
	return new_ptr;
}

/* print the statics about the apply and free of chunks.*/
void print_statics(){
	int i = 0;
	uint64 apply_count, free_count;
	while((++i) <= BIGGEST_CAP)	{
		apply_count = extra_apply[i];
		free_count = extra_free[i];
		if(i >= SMALL && i <= BIG ){
			apply_count += pre_alloc_apply[i - SMALL];
			free_count += pre_alloc_free[i - SMALL];	
		}
		printf("dc_mm : statics, %s%10d%s%10ld%s%10ld%s\n","chunks with specific capacity ",
			   POW2(i)," were applied ", apply_count ," times and freed ",free_count," times.");	
	}
}

uint32 first_bit(uint32 n)
{
	if(n == 0)
		return 0;
	uint32 c = 32;
	if (!(n & 0xffff0000)){
   		c -= 16;
  		n <<= 16;
  	}
  	if (!(n & 0xff000000)){
  		c -= 8;
  		n <<= 8;
  	}
  	if (!(n & 0xf0000000)){
  		c -= 4;
  		n <<= 4;
  	}
 	 if (!(n & 0xc0000000)){
  		c -= 2;
  		n <<= 2;
  	}
  	if (!(n & 0x80000000)){
  		c -= 1;
  	}
  	return c;
}