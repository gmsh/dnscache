

/********************************************************************
 * filename:		dl_cache_stub.c
 * author: 		synckey
 * version:		v1.0
 * datetime:		2011-06-29 19:54:43
 * description:		change it
 ********************************************************************/
#include "dl_cache_stub.h"
#include <arpa/inet.h>
fake_data_t fake_data ;

/*
 * To initialize the dl_cache.
 * size_per_data is the size of data.
 * In the stub, please call dl_cache_init(sizeof(fake_data));
 */
void dl_cache_init(size_t size_per_data){
	/* do nothing here.*/
}

/*
 * To get the data by the given key. The function return the pointer to
 * the data correspond to the key if hit, otherwise, that is a miss,
 * return NULL.
 * the key is a string end with '\0'.
 * Note that after copying the data by call this function, you _must_
 * call the unlock_after_copy().
 */
void * get_data_and_lock(uint8 * key)

{
	inet_pton(AF_INET, "192.168.1.1",  &fake_data.ip ) ; 
	fake_data.timestamp = time(NULL);; 
//	if ('w' == *key)
		return &fake_data; 
//	return NULL; 
}

/*
 * To unlock the cell after copying the data by call the above function.
 */
void unlock_after_copy(uint8 * key){
	/* do nothing here.*/
}

/*
 * To write a <key, data> pair to the cache.
 */
void push_dc(uint8 * key, void * data){
	/* do nothing here.*/
}

/***************  END OF dl_cache_stub.c  **************/
