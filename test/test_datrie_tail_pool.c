/*
 * test_datrie_tail_pool.c
 *
 *  Created on: Jun 26, 2011
 *      Author: gmsh
 */

#include "datrie_tail_pool.h"
char buff[80];
void replace_n(){
	int i;
	for(i = 0; i < 80; i ++){
		if(buff[i]=='\n'){
			buff[i] = '\0';
			break;
		}
	}
	buff[79] = '\0';
}
void output(datrie_tail_pool * pool)
{
  /*
   * get the next_free value.
   */
  uint32 size = (uint32)*(uint32 *)((uint8 **)pool + 1);
  uint32 i;
  for(i = 0; i < size; i++){

    if('\0' == ((uint8 *)(*(uint8 **)pool))[i])
      putchar('\n');
    else
      putchar(((uint8 *)(*(uint8 **)pool))[i]);
  }
}
int main()
{
	datrie_tail_pool * dtp = new_datrie_tail_pool();
	FILE * domains = fopen("./uniq_domain.list", "r");
	while(NULL != fgets(buff, 80, domains)){
	  replace_n();
	  dt_push_tail(buff, dtp);
	}
	fclose(domains);
	FILE * to_save = fopen("./out.tail", "rw");
	long val = write_datp_to_file(to_save, 0, dtp);
	fclose(to_save);
	output(dtp);
      	return 0;
}
