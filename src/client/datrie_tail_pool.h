/*
 * datire_tail_pool.h
 *
 *  Created on: Jun 26, 2011
 *  Author: gmsh
 *
 *  DATrie tail pool for keeping suffixes.
 *	Note that in the initial implementation, byte order is
 *	platform relative. The Tail save the symbols not the
 *	code, because both a symbol and a code is actually
 *	uint8. get_char(get_code(i)) = i.
 */

#ifndef DATIRE_TAIL_H_
#define DATIRE_TAIL_H_

#include <stdlib.h>
#include <stdio.h>
#include "typedefs.h"

#define likely(x) (x)
#define unlikely(x) (x)

typedef struct _datrie_tail_pool datrie_tail_pool;

/*
 * To get a new tail.
 */
datrie_tail_pool *new_datrie_tail_pool ();

/*
 * To read a pool from file.
 * The offset is relative to the start of file.
 * RETURN NULL if fails.
 */
datrie_tail_pool *read_datp_from_file (FILE * f, long offset);

/*
 * To write the pool into file
 * The offset is relative to the start of file.
 * RETURN -1 if fails.
 * RETURN how many bytes have been write if successful.
 */
long write_datp_to_file (FILE * f, long offset, datrie_tail_pool * pool);

/*
 * To push tail into the pool.
 * Tail is a string of uint8 end with '\0'.
 * Return index of the tail.
 * The index is larger than zero.
 */
int32 dt_push_tail (uint8 * tail, datrie_tail_pool * pool);

/*
 * To return tail by a given index.
 * the index _must_ be a valid one, otherwise the return is
 * meaningless.
 */
uint8 *dt_get_tail (int32 index, datrie_tail_pool * pool);

/*
 * To remove tail from the pool.
 */
void dt_remove_tail (int32 index, datrie_tail_pool * pool);

/*
 * To free the tail pool.
 */
void free_datp (datrie_tail_pool * pool);

#endif /* DATIRE_TAIL_H_ */
