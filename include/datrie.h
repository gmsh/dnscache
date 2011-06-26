/*
 * datrie.h
 *  This is an implementation of double-array structure
 *  for representing trie.
 *  For a transition from state s to t which takes
 *  character c as the input, the condition maintained
 *  in the double-array trie is:
 * 		check[base[s] + c] = s
 *		base[s] + c = t
 *	In  order to  keep a  node  number  from  exceeding the
 *  maximum index of the double-array,  we define the  size,
 *	denoted by DA-SIZE, of the double-array as the maximum
 *	index of the nonzero entries of CHECK, and DA-SIZE is
 *	stored  in check[1] for the dynamic double-array.
 *  Created on: Jun 25, 2011
 *      Author: gmsh
 */

#ifndef DATRIE_H_
#define DATRIE_H_

typedef struct _da_cell da_cell;

typedef struct _double_array double_array;

typedef struct _tail_block tail_block;

typedef struct _tail tail;

#endif /* DATRIE_H_ */
