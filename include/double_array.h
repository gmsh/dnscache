/*
 * double_array.h
 *
 *  Created on: Jun 26, 2011
 *      Author: gmsh
 *
 *  This is an implementation of double-array structure
 *  for representing trie.
 *  For a transition from state s to t which takes
 *  character c as the input, the condition maintained
 *  in the double-array trie is:
 * 		check[base[s] + c] = s
 *		base[s] + c = t
 *	In order to manage the free cells, let r1 r2 r3 ...
 *	rm be the free cells in the double-array structure,
 *	ordered by position.
 *		check[0] = -r1
 *		check[ri] = -r( i + 1 )
 *		1 <=i <= m -1
 *		check[rm] = 0;
 *		base[0] = -rm;
 *		base[r1] = -r0;
 *		base[ri] = -r( i - 1)
 *  cell[1] is the root cell.     
 *
 *	Note that in the initial implementation, byte order is
 *	platform relative, and the data pointer is set to zero
 *	if the double_array is read from a file, because it is
 *	useless.
 */

#ifndef DOUBLE_ARRAY_H_
#define DOUBLE_ARRAY_H_

#include <stdio.h>
#include <stdlib.h>

typedef struct _double_arrary double_array;

/*
 * To get a pointer to new double_array
 * return NULL if failed.
 */
double_array * new_double_array();

/*
 * To read a double_array form file.
 * offset is relative to the start of file.
 * return NULL If failed.
 */
double_array * read_da_from_file(FILE * f, long offset);

/*
 * To write da to file f.
 * offset is relative to the start of file.
 * return how many bytes have been write if successful.
 * Otherwise, -1 is returned.
 */
long write_da_to_file(FILE * f, long offset, double_array * da);

/*
 * To insert <key, data> to the double array.
 * The key is a uint8 string end with '\0'.
 * Every element of key must be in the charset defined
 * in dc_set.h.
 * the data pointer can be NULL.
 */
void da_insert(uint8 * key, void * data, double_array * da);

/*
 * To delete the key from given double_array.
 */
void da_delete(uint8 * key, double_array * da);

/*
 * To set <key, data> in a given double_array.
 * If key doesn't exists in da, this function behaves like
 * da_insert().
 */
void da_set_data(uint8 * key, void * data, double_array * da);

/*
 * return the data correspond to a given key.
 * If the key doesn't exists in da, NULL is returned.
 * Of course if data is NULL, NULL is returned too.
 */
void * da_get_data(uint8 * key, double_array *da);

/*
 * To free the da.
 */
void free_da(double_array * da);
#endif /* DOUBLE_ARRAY_H_ */
