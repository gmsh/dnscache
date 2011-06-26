/*
 * datrie_tail_pool.c
 *
 *  Created on: Jun 26, 2011
 *      Author: gmsh
 */

#include "dc_mm.h"
#include "constants.h"
struct _datrie_tail_pool{
	uint8 * pool;
};
datrie_tail_pool * new_datrie_tail_pool(){
	datrie_tail_pool * to_return = dc_alloc(sizeof(datrie_tail_pool));
	to_return->pool = dc_alloc(sizeof(uint8) * INITIAL_DATRIE_TAIL_POOL_SIZE);
	return to_return;
}
