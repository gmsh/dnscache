#ifndef __ALPHA_MAP_H
#define __ALPHA_MAP_H

#include <stdio.h>

#include "typedefs.h"
#include "triedefs.h"

/**
 * @file alpha-map.h
 * @brief AlphaMap data type and functions
 */

/**
 * @brief AlphaMap data type
 */
typedef struct _AlphaMap AlphaMap;

AlphaMap *alpha_map_new ();

AlphaMap *alpha_map_clone (const AlphaMap * a_map);

void alpha_map_free (AlphaMap * alpha_map);

int alpha_map_add_range (AlphaMap * alpha_map,
			 AlphaChar begin, AlphaChar end);

int alpha_char_strlen (const AlphaChar * str);

#endif /* __ALPHA_MAP_H */

