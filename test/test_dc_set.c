/*
 * test_dc_set.c
 *
 *  Created on: Jun 25, 2011
 *      Author: gmsh
 */


#include "dc_set.h"
#include <stdio.h>
int main(){
	/*
	 * forward test: get_char
	 */
	printf("code\tchar\n");
	uint8 i;
	for(i = 1; i < 39; i++){
		printf("%d\t%c\n", i, get_char(i));
	}
	/*
	 * backward test: get_code
	 */
	printf("char\tcode\n");
	for(i = 97; i < 123; i++){
		printf("%c\t%d\n", i, get_code(i));
	}
	printf("%c\t%d\n", 45, get_code(45));
	printf("%c\t%d\n", 46, get_code(46));
	for(i = 48; i < 58; i++){
		printf("%c\t%d\n", i, get_code(i));
	}
	return 0;
}
