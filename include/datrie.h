/* * datrie.h
 *
 *  Created on: Jun 25, 2011
 *  Author: gmsh
 *
 *  DATrie is a kind of dictionary search tree.
 *
 *  Note that in the initial implementation, byte order is
 *	platform relative, and the data pointer is set to zero
 *	if the double_array is read from a file, because it is
 *	useless.
 */

#ifndef DATRIE_H_
#define DATRIE_H_

#include "datrie_tail_pool.h"
#include "double_array.h"

typedef struct _datrie datrie;

/*
 * To get a pointer to new datrie
 * return NULL if failed.
 */
datrie * new_datrie();

/*
 * To read a datrie from file.
 * NULL if failed.
 */
datrie * read_datrie(char * filename);

/*
 * To write a datrie to file.
 * RETURN 0 if successful, otherwise -1.
 */
int write_datrie(char * filename);

/*
 * To insert <key, data> to the datrie.
 * The key is a uint8 string end with '\0'.
 * Every element of key must be in the charset defined
 * in dc_set.h.
 * the data pointer can be NULL.
 */
void datrie_insert(uint8 * key, void * data, datrie * ptr);

/*
 * To delete the key from given datrie.
 */
void datrie_delete(uint8 * key, datrie * ptr);

/*
 * To set <key, data> in a given double_array.
 * If key doesn't exists, this function behaves like
 * datrie_insert().
 */
void da_set_data(uint8 * key, void * data, datrie * ptr);

/*
 * return the data correspond to a given key.
 * If the key doesn't exists in ptr, NULL is returned.
 * Of course if data is NULL, NULL is returned too.
 */
void * da_get_data(uint8 * key, datrie * ptr);

/*
 * To free the datrie.
 */
void free_datrie(datrie * ptr);

#endif /* DATRIE_H_ */
