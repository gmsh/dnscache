/*
 * datrie_tail_pool.c
 *
 *  Created on: Jun 26, 2011
 *      Author: gmsh
 */
#define likely(x)       __builtin_expect((x),1)
#define unlikely(x)     __builtin_expect((x),0)
#include "dc_mm.h"
#include "constants.h"
#include "datrie_tail_pool.h"
#include <string.h>
struct _datrie_tail_pool{
	uint8 * pool;
	/*
	 * TODO memory waste of use next_free.
	 */
	uint32 next_free;
	uint32 pool_size;
};
datrie_tail_pool * new_datrie_tail_pool()
{
	datrie_tail_pool * to_return = dc_alloc(sizeof(datrie_tail_pool));
	to_return->pool = dc_alloc(sizeof(uint8) * DATRIE_TAIL_POOL_INITIAL_SIZE);
	to_return->next_free = 0;
	to_return->pool_size = DATRIE_TAIL_POOL_INITIAL_SIZE;
	return to_return;
}

int32 dt_push_tail(uint8 * tail, datrie_tail_pool * pool)
{
	uint32 index = pool->next_free;
	if(unlikely(strlen((char*) tail) /* -1 for '\0' */
			< pool->pool_size - pool->next_free -1)){
		pool->pool_size = pool->pool_size * DATRIE_TAIL_POOL_INCREASING_RATE;
		pool->pool = dc_realloc(pool->pool, pool->pool_size);
	}
	strcpy(pool->pool, tail);
	pool->next_free = pool->next_free + strlen((char *)tail) + 1;
	return index;
}

uint8 * dt_get_tail(int32 index, datrie_tail_pool * pool)
{
	return pool->pool + index;
}

void dt_remove_tail(int32 index, datrie_tail_pool * pool)
{
	/* for the current implementation this methods is useless. */
}

void free_datp(datrie_tail_pool * pool)
{
	dc_free(pool->pool);
	dc_free(pool);
}

long write_datp_to_file(FILE * f, long offset, datrie_tail_pool * pool)
{
	/* stub */
	return -1;
}

datrie_tail_pool * read_datp_from_file(FILE * f, long offset)
{
	/* stub */
	return NULL;
}
