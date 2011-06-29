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


int main(){
	struct slist * sl;
	struct sl_node * ptr;
	size_t i = 5;
	sl = mk_slist(malloc,i);	
	while(i--){
		ptr->data = malloc(sizeof(int));
		*(int *)(ptr->data)=i;	
		push(ptr->data,sl);	
	}/* 依次遍历赋值 */
	traverse(print_data,sl);
=======
	printf("---------测试一开始-----------\n");
	struct slist * test_list= mk_slist((uint32 *)malloc(sizeof(uint32)),CAPACITY);	
	struct sl_node * ptr;//=(struct sl_node *)dc_alloc(sizeof(struct sl_node ));
	size_t i=0;
	size_t delta=5;

	for(i=0; i < CAPACITY; i++){
		int *data = malloc(sizeof(int));
		*data = i;
		//ptr = malloc(sizeof(sl_node));
		//ptr->data = data;
		append(data, test_list);

		
	}

	/* 依次遍历赋值 */
	//print_data(*ptr);
	printf("test pop result\n");
	pop(test_list);
	printf("test push result\n");
	push(100,test_list);
	printf("test append result\n");	
	append(101,test_list);
	printf("test expand result\n");
	sl_expand(test_list,(uint32 *)malloc(sizeof(uint32)),delta);
	printf("test traverse result\n");
	traverse(print_data(ptr),test_list);
	printf("test sl_free result\n");
	sl_free(free,test_list);
	traverse(print_data(ptr),test_list);
	//traverse((* visit)(*ptr),test_list);	
	
>>>>>>> 0c505330203b20e964a84507308a7b3880f3d3b3
}

/* print the data */
void print_data(struct sl_node * sn){
	printf("%d\n",*(uint32 *)(sn->data));	
}
