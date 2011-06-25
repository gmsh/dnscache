/*
 * datrie.h
 *  This is an implementation of double-array structure
 *  for representing trie.
 *  For a transition from state s to t which takes
 *  character c as the input, the condition maintained
 *  in the double-array trie is:
 * 		check[base[s] + c] = s
 *		base[s] + c = t
 *  Created on: Jun 25, 2011
 *      Author: gmsh
 */

#ifndef DATRIE_H_
#define DATRIE_H_

typedef struct {
	uint32 base;
	uint32 check;
	void * data;
} array_element;

#endif /* DATRIE_H_ */
