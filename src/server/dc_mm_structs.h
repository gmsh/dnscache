/*
 * filename:	mm_struct.c
 * author:	wakemecn
 * date:	6/23/2011
 * description: define structs used for memory management.
 */

#ifndef _MM_STUCT_H
#define _MM_STUCT_H

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <pthread.h>
#include "typedefs.h"
#include "constants.h"

/* 
 * manage the chunks with specific capacity.
 */
struct chunks_manager {	
	/* 
	 * the defined n for the capacity of  2^n-bytes ch-
	 * unks.
	 */
	uint64	chunks_cap;	
	/*  the count of idle chunks. */
	uint16	idle_num;	
	/* the list of idle chunk or chunks. */
	struct slist * idle_chunks;	
	/* the list of allocated chunk or chunks. */
	struct slist * alloced_chunks;	
}	* chunks_manager_table[CHUNK_TYPE_NUM];

/* 
 * manager the extra list of chunks, as the name shows that
 * num in []  presents the cap.
 */
struct extra_list_manager{
	/* idle number */
	uint16 idle_num;
	/* total alloc number */
	uint16 total_num;	
	/* pointer to the list */
	struct slist * chunks_list;
}	* elm_table[BIGGEST_CAP + 1];

/*
 * Statistical data used for configure pre-allocted meomery.
 */
uint64 pre_alloc_apply[CHUNK_TYPE_NUM], pre_alloc_free[CHUNK_TYPE_NUM];
uint64 extra_apply[BIGGEST_CAP + 1], extra_free[BIGGEST_CAP + 1];

/*
 * Synchronous mutex.
 */
pthread_mutex_t * pre_alloc_mutex[CHUNK_TYPE_NUM];
pthread_mutex_t * extra_mutex[BIGGEST_CAP + 1];

/*
 * return the position of the first bit in a binary number.
 */
uint32 first_bit(uint32);

#endif
