#ifndef __ALPHA_MAP_PRIVATE_H
#define __ALPHA_MAP_PRIVATE_H

#include <stdio.h>
#include "alpha-map.h"

AlphaMap *alpha_map_fread_bin (FILE * file);

int alpha_map_fwrite_bin (const AlphaMap * alpha_map, FILE * file);

TrieChar alpha_map_char_to_trie (const AlphaMap * alpha_map, AlphaChar ac);

AlphaChar alpha_map_trie_to_char (const AlphaMap * alpha_map, TrieChar tc);

TrieChar *alpha_map_char_to_trie_str (const AlphaMap * alpha_map,
				      const AlphaChar * str);

AlphaChar *alpha_map_trie_to_char_str (const AlphaMap * alpha_map,
				       const TrieChar * str);


#endif /* __ALPHA_MAP_PRIVATE_H */

