/*
 * dl_cache_stub.h
 *
 *	This header file could be used by the dc_server as a stub,
 *	when the real dl_cache lib is under heavy development.
 *	It is promised that the api would not changed dramatically.
 *  Created on: Jun 26, 2011
 *      Author: gmsh
 */

#ifndef DL_CACHE_STUB_H_
#define DL_CACHE_STUB_H_

#include <stdlib.h>
/*
 * The below is the fake data, you should modify it if it does not
 * meet the demand.
 */
struct {
	uint32 a;
	uint16 b;
} fake_data;

/*
 * To get the data by the given key. The function return the pointer to
 * the data correspond to the key if hit, otherwise, that is a miss,
 * return NULL.
 * the key is a string end with '\0'.
 * Note that after copying the data by call this function, you _must_
 * call the unlock_after_copy().
 */
void * get_data_and_lock(uint8 * key){
	if ('w' == *key)
		return NULL; /* stub for miss */
	return &fake_data; /* hit */
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

#endif /* DL_CACHE_STUB_H_ */
