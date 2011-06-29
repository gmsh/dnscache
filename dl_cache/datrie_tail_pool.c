/*
 * datrie_tail_pool.c
 *
 *  Created on: Jun 26, 2011
 *      Author: gmsh
 */
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
  while(unlikely(strlen((char*) tail) + 1 /* +1 for '\0'
					      * may _not_ use to -1 in the right
					      * part because the result is uint32
					      */
		 > pool->pool_size - pool->next_free)){
    pool->pool_size = pool->pool_size * DATRIE_TAIL_POOL_INCREASING_RATE;
    pool->pool = dc_realloc(pool->pool, pool->pool_size);
  }
  strcpy(pool->pool + pool->next_free, tail);
  pool->next_free = pool->next_free + strlen((char *)tail) + 1;
#ifdef _DC_DEBUG
  printf("%d\t%d\n", pool->next_free, pool->pool_size);
#endif
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
  if(NULL == pool)
    return;
  if(NULL != pool->pool)
    dc_free(pool->pool);
  dc_free(pool);
}

long write_datp_to_file(FILE * f, long offset, datrie_tail_pool * pool)
{
  long size = fseek(f, offset, SEEK_SET);
  if(unlikely(size < 0))
    return -1;
  size += fwrite(&(pool->next_free), sizeof(uint32), 1, f);
  if((unlikely(size != 1)))
    return -1;
  /*
   * because return is count by bytes, to multiply sizeof(uint32)
   */
  size *= sizeof(uint32);
  size += fwrite(pool->pool, sizeof(uint8), pool->next_free, f);
  return size;
}

datrie_tail_pool * read_datp_from_file(FILE * f, long offset)
{
  size_t temp = fseek(f, offset, SEEK_SET);
  if(unlikely(temp < 0))
    return NULL;
  datrie_tail_pool * to_return = dc_alloc(sizeof(datrie_tail_pool));
  temp = fread(&(to_return->next_free), sizeof(uint32), 1, f);
  if(unlikely(temp == 0)){
    dc_free(to_return);
    return NULL;
  }
  to_return->pool = (uint8 *)dc_alloc(to_return->next_free * sizeof(uint8));
  to_return->pool_size = to_return->next_free;
  temp = fread(to_return->pool, sizeof(uint8), to_return->next_free, f);
  if(unlikely(temp == 0)){
    dc_free(to_return->pool);
    dc_free(to_return);
    return NULL;
  }
  return to_return;
}
