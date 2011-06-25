/*
 * filename:	test_slist.c
 * author:		wakemecn
 * description: the test program for slist created by gmsh
 * date:		Jun 25,2009
 */

#define	CAPACITY	5

#include "slist.h"
#include "typedefs.h"

void print_data(struct sl_node *);

int main(){
	struct slist * test_list;
	struct sl_node * ptr;
	size_t i=0;
	test_list = mk_slist((uint32 *)malloc(sizeof(uint32)),CAPACITY);	
	while((++ptr) < (test_list->head + test_list->length)){
		*(int * )(ptr->data)=i++;			
	}
}

/* print the data */
void print_data(struct sl_node * sn){
	printf("%d\n",*(uint32 *)(sn->data));	
}
