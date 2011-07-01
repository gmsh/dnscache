#include "dc_mm.h"
#include <math.h>

#define POW2(x) (0x0000000000000001 << x)

main(){
	mm_init();
	void * ptr[2000];
	int i, j;int count=20;
while(1){
	while(--count < 20 && count > 18){
		for(i = 0; i < 2; i++)
			ptr[i] = dc_alloc(POW2(count));
		print_statics();
//	short x=1, y;
//		while(x++!=0){
//			y=x * x;
//		}
//		y = y * 5;
//		printf("%d\n",y);
		for(j = 0; j < 2; j++ )
			dc_free(ptr[j]);
		print_statics();
	}

	while(--count < 20 && count > 18){
		for(i = 0; i < 2; i++)
			ptr[i] = dc_alloc(POW2(count));
		print_statics();
//		short x=1, y;
///		while(x++!=0){
//			y=x * x;
//		}
//		y = y * 5;
//		printf("%d\n",y);
		for(j = 0; j < 2; j++ )
			dc_free(ptr[j]);
		print_statics();
	}

	print_statics();
	count = 20;
}
}
