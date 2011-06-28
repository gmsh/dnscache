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
#define POW2(x) (0x00000001 << x)
#define	LOG2(x) ffs(x)
#define DEFAULT_EXTRA 4	/* default number of each type of extra chunks  */

#include <stdlib.h>
#include <math.h>
#include "typedefs.h"
#include "slist.h"

struct chunk { /* manage the chunk */
	uint64 chunk_cap; /* the capacity of a chunk */
	void * mem;		/* the pointer to the memory */ 
}

struct chunks_manager {	/* manage the chunks with small capacity. */
	uint32	chunks_cap;	/* the defined n for the capacity of  2^n-bytes chunks */
	uint16	idle_num;	/*  the count of pre-alllocated chunks */
	struct slist * idle_chunks;	/* the list of idle chunk or chunks */
	struct slist * alloced_chunks;	/* the list of allocated chunk or chunks */
}	* chunks_manager_table[CHUNK_TYPE_NUM];

struct extra_list_manager{	/* manager the extra list of chunks */
	uint16 idle_num;	/* idle number */
	uint16 total_num;	/* total alloc num */
	struct slist * chunks_list;/* pointer to the list */
}	* elm_table[30];/* as the name shows that num in []  presents the cap*/

uint16 num_each_chunks[]={ 64, 64, 64, 64, 64,
                           32, 32, 32, 32, 32,
			   16, 16,  8 };
			   /* number of each type of chunks */

/* uint16 num_extra_chunks[]={  8,  8,  8,  8,  8,
 *                            4,  4,  4,  4,  4,
 * 			    2,  2,  2 };
 */
				/* number of each type of extra chunks */

struct slist * idle_chunks_table[CHUNK_TYPE_NUM];  /* array of pointers to
                                                 * idle chunks lists.
						 */
/*
 * struct slist * alloced_chunks_table[CHUNK_TYPE_NUM]; 
 */			
  			/* array of pointers 
                                                   * to allocated chunks
						   * lists.
						   */

/*
 * return the first bit filled with 1's place.
 */
uint32 ffs(uint32 n)
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
  	return c;
}

#endif
