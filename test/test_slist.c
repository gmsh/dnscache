/*
 * filename:	test_slist.c
 * author:		wakemecn
 * description: the test program for slist created by gmsh
 * date:		Jun 25,2009
 */

#define	CAPACITY	5

#include "slist.h"
#include "typedefs.h"
#include <stdlib.h>
#include "dc_mm.h" 

void print_data(struct sl_node * sn);

int main(){
	struct slist * sl;
	void * ptr;
	size_t i = 10;
	sl = mk_slist(malloc,3);	
	while(i--){
		ptr = malloc(sizeof(int));
		*(int *)(ptr)=i;
		if(push(ptr,sl))
			break;	
	}/* 依次遍历赋值 */
	traverse(print_data,sl);
	void * x;
	while(x = (pop(sl))){
		printf("pop %d\n",*(int *)x);
	}
	*(int *) ptr = 100;
	append(ptr,sl);
    sl = sl_expand(sl,malloc,2);
	int a[] = {
		99, 98, 97, 96, 95
		};
	append(&a[1],sl);
	append(&a[2],sl);
	traverse(print_data,sl);
	mv2head(&a[1],sl);
	traverse(print_data,sl);
	printf("cap: %d, len : %d\n",sl->capacity,sl->length);
	sl_free(free,sl);
	traverse(print_data,sl);
}

/* print the data */
void print_data(struct sl_node * sn){
	printf("%d\n",*(uint32 *)(sn->data));	
}
