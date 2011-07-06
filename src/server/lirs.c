

/********************************************************************
 * filename:		lirs.c
 * author: 		synckey
 * version:		v1.0
 * datetime:		2011-07-04 13:18:05
 * description:		the lirs algritim
 ********************************************************************/
#include "lirs.h"

uint64 	n_nodes;	/* the number of nodes */
#ifdef LOCK_FREE
hash_table hash_table[N_NODES];
static void insert_to_head(data_t *data);
static unsigned int BKDRHash(char *str);
static void move_to_head(dlist_node *);


void init()
{
	n_nodes = 0;
	head = NULL;
	tail = NULL;
	uint64 i;
	for(i = 0;i < N_NODES; i++)
		hash_table.dlist_node = NULL;
	

	return;
}

data_t* read_data(uint8 *key)
{	
	uint32 index = BKDRHash(key);
	if(NULL == hash_table[index]->node)
		return NULL;
	move_to_head(hash_table[index].node);
	return hash_table[index].node->data;
}

static void move_to_head(dlist_node *node)
{	
	if(head == node)	/* only one node */
		return;
	
	if(NULL != node.next)
		node.pre -> next = node.next;
	else{	/* node is the tail */
		node.pre -> next = NULL;
		tail = node.pre;
	}
	if(NULL != node.pre)
		node.next -> pre = node.pre;
	node.next = head;
	node.pre = NULL;
	head = node;
	return;
}


/*
 * if the list is full, delete the data of the tail,
 * move data to head.
 * if the list is not full, insert it 
 */
void	write_data(uindata_t *data)
{
	uint32 index = BKDRHash(key);
	if(NULL == hash_table[index]){
		dlist_node *node = (dlist_node *)dc_alloc(sizeof(dlist_node));
		node -> index = index;
		node -> data =(data_t *) malloc(sizeof(data_t));
		node -> data->ip = data->ip;
		node -> data->timestamp = data->timestamp;
		node -> feature = 0; //todo;
		insert_to_head(node);
		return;
	}
	//crash
	hash_table[index] -> data -> ip = data -> ip;
	hash_table[index] -> data -> timestamp = data -> timestamp;
	move_to_head(hash_table[index]);
	return;
		
}



/* 
 * BKDR Hash Function
 * from  the internet
 *
 */
static uint32 BKDRHash(charn *str)
{
	uint32 seed = 131; // 31 131 1313 13131 131313 etc..
        uint32 hash = 0;
	while (*str)
		hash = hash * seed + (*str++);

	return (hash & 0x7FFFFFFF);
}
#endif
/***************  END OF lirs.c  **************/
