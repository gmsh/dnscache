/*
 * filename:	mm_struct.c
 * author:	wakemecn
 * date:	6/23/2011
 * description: define structs used for memory management.
 */

#ifndef _MM_STUCT_H
#define _MM_STUCT_H

#define SMALL 8			                /* the 2^SMALL is the smallese chunks */
#define	BIG	20			        /* the 2^BIG is the biggest chunks */
#define CHUNK_TYPE_NUM ( BIG - SMALL + 1 )	/* counts of  chunk's type */
#define DEFAULT_EXTRA 4	/* default number of each type of extra chunks  */
#define BIGGEST_CAP 30

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <pthread.h>
#include "typedefs.h"
#include "slist.h"

struct chunks_manager {	/* manage the chunks with small capacity. */
	uint64	chunks_cap;	/* the defined n for the capacity of  2^n-bytes chunks */
	uint16	idle_num;	/*  the count of pre-alllocated chunks */
	struct slist * idle_chunks;	/* the list of idle chunk or chunks */
	struct slist * alloced_chunks;	/* the list of allocated chunk or chunks */
}	* chunks_manager_table[CHUNK_TYPE_NUM];

struct extra_list_manager{	/* manager the extra list of chunks */
	uint16 idle_num;	/* idle number */
	uint16 total_num;	/* total alloc num */
	struct slist * chunks_list;/* pointer to the list */
}	* elm_table[BIGGEST_CAP + 1];/* as the name shows that num in []  presents the cap*/


struct slist * idle_chunks_table[CHUNK_TYPE_NUM];  /* array of pointers to
                                                 * idle chunks lists.
						 */

pthread_mutex_t * pre_alloc_mutex[CHUNK_TYPE_NUM];
pthread_mutex_t * extra_mutex[BIGGEST_CAP + 1];

#endif
