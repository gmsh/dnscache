/*
 * test__typdef.c
 *
 *  Created on: Jun 25, 2011
 *      Author: gmsh
 */
#include "typedefs.h"
#include <stdio.h>
int main()
{
	printf("sizeof(unit8)\t%lu\n", sizeof(uint8));
	printf("sizeof(uint16)\t%lu\n", sizeof(uint16));
	printf("sizeof(uint32)\t%lu\n", sizeof(uint32));
	printf("sizeof(uint64)\t%lu\n", sizeof(uint64));
	return 0;
}
