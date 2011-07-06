

/********************************************************************
 * filename:		lirs.h
 * author: 		synckey
 * version:		v1.0
 * datetime:		2011-07-04 11:08:05
 * description:		
 ********************************************************************/
#include "constants.h"
#include "dl_kache.h"	/* data_t */
#include "dc_mm.h"	/* data_t */

#define	  N_NODES	(1024 * 1024)   /* number of dlist_node */


 
/* node to store info */
typedef struct  {
	uint32	index;		/* the array index*/
	data_t	*data;		/* ip and time */
	struct dlist_node *next;
	struct dlist_node *pre;
	uint32	feature;	/* in case of crash */
}dlist_node ;

typedef struct {
	dlist_node *node;		
} hash_table;



dlist_node	*head;
dlist_node	*tail;
uint64		n_nodes;

/**
 * init this module
 */
 
void init();

/**
 * read data from the dlist.
 */
data_t* read_data(uint8 *,uint8 *);


/**
 * write data 
 */
void store(uint8 * key, data_t * data);







/***************  END OF lru.c  **************/
