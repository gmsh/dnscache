/*
 * dc_set.h
 *	CharSet for domain names.
 *	∑ = {a-z 0-9 - . '\0'}
 *	'\0' is the end of key.
 *	$ is the empty input char.
 *	There are 38(=26 + 10 + 2) characters.
 *	The code 0 is reserved.
+---------+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
|character| a| b| c| d| e| f| g| h| i| j| k| l| m| n| o| p| q| r| s| t| u|
+---------+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
|  code   | 1| 2| 3| 4| 5| 6| 7| 8| 9|10|11|12|13|14|15|16|17|18|19|20|21|
+---------+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
|character| v| w| x| y| z| -| .| 0| 1| 2| 3| 4| 5| 6| $| 8| 9|\0| 7|
+---------+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
|  code   |22|23|24|25|26|27|28|29|30|31|32|33|34|35|36|37|38|39|40|
+---------+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *
 *  Created on: Jun 25, 2011
 *      Author: gmsh
 */

#ifndef DC_SET_H_
#define DC_SET_H_
#include "typedefs.h"
/*
 * get the character by code;
 */
inline uint8 get_char(uint8 code);

/*
 * get the code by character;
 */
inline uint8 get_code(uint8 character);
#endif /* DC_SET_H_ */
