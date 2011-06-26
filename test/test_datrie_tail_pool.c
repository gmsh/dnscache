/*
 * test_datrie_tail_pool.c
 *
 *  Created on: Jun 26, 2011
 *      Author: gmsh
 */

#include "datrie_tail_pool.h"
#include "../dl_cache/datrie_tail_pool.c"
char buff[80];
void replace_n(){
	int i;
	for(i = 0; i < 80; i ++){
		if(buff[i]=='\n'){
			buff[i] = '\0';
			break;
		}
	}
}
int main()
{
	datrie_tail_pool * dtp = new_datrie_tail_pool();
	FILE * domains = fopen("./uniq_domain.list", "r");
	while(NULL != fgets(buff, 80, domains));
	fclose(domains);
	while(1);/* check memory use.*/
	return 0;
}
