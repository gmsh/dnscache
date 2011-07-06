#ifndef __TRIE_H
#define __TRIE_H

#include "triedefs.h"
#include "alpha-map.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @file trie.h
 * @brief Trie data type and functions
 */

/**
 * @brief Trie data type
 */
  typedef struct _Trie Trie;

/**
 * @brief Trie enumeration function
 *
 * @param key  : the key of the entry
 * @param data : the data of the entry
 *
 * @return TRUE to continue enumeration, FALSE to stop
 */
  typedef Bool (*TrieEnumFunc) (const AlphaChar * key,
				TrieData key_data, void *user_data);

/**
 * @brief Trie walking state
 */
  typedef struct _TrieState TrieState;

/*-----------------------*
 *   GENERAL FUNCTIONS   *
 *-----------------------*/

  Trie *trie_new (const AlphaMap * alpha_map);

  Trie *trie_new_from_file (const char *path);

  Trie *trie_fread (FILE * file);

  void trie_free (Trie * trie);

  int trie_save (Trie * trie, const char *path);

  int trie_fwrite (Trie * trie, FILE * file);

  Bool trie_is_dirty (const Trie * trie);


/*------------------------------*
 *   GENERAL QUERY OPERATIONS   *
 *------------------------------*/

  Bool trie_retrieve (const Trie * trie,
		      const AlphaChar * key, TrieData * o_data);

  Bool trie_store (Trie * trie, const AlphaChar * key, TrieData data);

  Bool trie_store_if_absent (Trie * trie, const AlphaChar * key,
			     TrieData data);

  Bool trie_delete (Trie * trie, const AlphaChar * key);

  Bool trie_enumerate (const Trie * trie,
		       TrieEnumFunc enum_func, void *user_data);


/*-------------------------------*
 *   STEPWISE QUERY OPERATIONS   *
 *-------------------------------*/

  TrieState *trie_root (const Trie * trie);


/*----------------*
 *   TRIE STATE   *
 *----------------*/

  TrieState *trie_state_clone (const TrieState * s);

  void trie_state_copy (TrieState * dst, const TrieState * src);

  void trie_state_free (TrieState * s);

  void trie_state_rewind (TrieState * s);

  Bool trie_state_walk (TrieState * s, AlphaChar c);

  Bool trie_state_is_walkable (const TrieState * s, AlphaChar c);

/**
 * @brief Check for terminal state
 *
 * @param s    : the state to check
 *
 * @return boolean value indicating whether it is a terminal state
 *
 * Check if the given state is a terminal state. A terminal state is a trie
 * state that terminates a key, and stores a value associated with it.
 */
#define   trie_state_is_terminal(s) trie_state_is_walkable((s),TRIE_CHAR_TERM)

  Bool trie_state_is_single (const TrieState * s);

/**
 * @brief Check for leaf state
 *
 * @param s    : the state to check
 *
 * @return boolean value indicating whether it is a leaf state
 *
 * Check if the given state is a leaf state. A leaf state is a terminal state 
 * that has no other branch.
 */
#define   trie_state_is_leaf(s) \
    (trie_state_is_single(s) && trie_state_is_terminal(s))

  TrieData trie_state_get_data (const TrieState * s);

#ifdef __cplusplus
}
#endif

#endif				/* __TRIE_H */

