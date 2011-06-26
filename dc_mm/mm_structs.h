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
#define CHUNCK_TYPE_NUM ( BIG - SMALL + 1 )	/* counts of  chunck's type */
#define POW2(x) pow(2,x)
#define	LOG2(x) (log(x)/log(2))
#define DEFAULT_EXTRA 4;	/* default number of each type of extra chunks  */

#include <math.h>
#include "typedefs.h"

struct chunck_manager {	/* manage the chunks with small capacity. */
	uint32	chunck_cap;	/* the defined n for the capacity of  2^n-bytes chunks */
	uint16	idle_num;	/*  the count of pre-alllocated chunks */
	slist * idle_chucks;	/* the list of idle chunk or chunks */
	slist * alloced_chucks;	/* the list of allocated chunk or chunks */
	slist * extra_chunks;   /* the dynamicly allocated chunks list */
}	chunk_manager_table[CHUNCK_TYPE_NUM];

struct extra_list_manager{	/* manager the extra list of chunks */
	uint16 idle_num;	/* idle number */
	slist * chunks_list;/* pointer to the list */
}	elm_table[30];/* as the name shows that num in []  presents the cap*/

uint16 num_each_chunks[]={ 64, 64, 64, 64, 64,
                           32, 32, 32, 32, 32,
			   16, 16,  8 }
			   /* number of each type of chunks */
uint16 num_extra_chunks[]={  8,  8,  8,  8,  8,
                            4,  4,  4,  4,  4,
			    2,  2,  2 }
				/* number of each type of extra chunks */

stuct slist idle_chunks_table[CHUNK_TYPE_NUM];  /* array of pointers to
                                                 * idle chunks lists.
						 */
stuct slist alloced_chunks_table[CHUNK_TUPE_NUM]; /* array of pointers 
                                                   * to allocated chunks
						   * lists.
						   */

#endif
