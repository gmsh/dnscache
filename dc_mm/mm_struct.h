/*
 * filename:	mm_struct.c
 * author:		wakemecn
 * date:		6/23/2011
 * description: define structs used for memory management.
 */

#ifndef _MM_STUCT_H
#define _MM_STUCT_H

#define	BIG	20			/* the 2^BIG is the biggest chunks */
#define SMALL 8			/* the 2^SMALL is the smallese chunks */
#define CHUNCK_TYPE_NUM ( BIG - SMALL + 1 )	/* counts of  chunck's type */


#include "typedefs.h"
//typedef 
struct chunck_manager {
	uint32	chunck_cap;			/* the defined n for the capacity of  2^n-bytes chunks */
	uint8	chunck_total_num;	/*  the count of pre-alllocated chunks */
	slist * idel_chucks;		/* the list of idle chunk or chunks */
	slist * alloced_chucks;		/* the list of allocated chunk or chunks */
}	chunk_m_table[CHUNCK_TYPE_NUM];

//struct 


#endif
