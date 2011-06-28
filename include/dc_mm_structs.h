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
#define POW2(x) (0x0000000000000001 << x)
#define	LOG2(x) ffs(x)
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

uint16 num_each_chunks[]={ 64, 64, 64, 64, 64,
                           32, 32, 32, 32, 32,
			   16, 16,  8 };
			   /* number of each type of chunks */

struct slist * idle_chunks_table[CHUNK_TYPE_NUM];  /* array of pointers to
                                                 * idle chunks lists.
						 */

/*
 * return the first bit filled with 1's place.
 */
uint64 ffs(uint32 n)
{
	if(n==0)
		return 0;
	uint8 c = 32;
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
  	return (uint64)c;
}

pthread_mutex_t * pre_alloc_mutex[CHUNK_TYPE_NUM];
pthread_mutex_t * extra_mutex[BIGGEST_CAP + 1];
#endif
