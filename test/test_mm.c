#include "dc_mm.h"
#include <math.h>

#define POW2(x) (0x0000000000000001 << x)

main(){
	mm_init();
	void * ptr[2000];
	int i, j;int count=25;
while(1){
//	while(--count < 25 && count > 1){
	count=10;
		for(i = 0; i < 40; i++)
			ptr[i] = dc_alloc(POW2(count));
		print_statics();
		for(j = 0; j < 40; j++ )
			dc_free(ptr[j]);
		print_statics();
		for(i = 0; i < 40; i++)
			ptr[i] = dc_alloc(POW2(count));
		print_statics();
		for(j = 0; j < 40; j++ )
			dc_free(ptr[j]);
		print_statics();
//	}
//	while(--count < 25 && count > 1){
		for(i = 0; i < 40; i++)
			ptr[i] = dc_alloc(POW2(count));
		print_statics();
		for(j = 0; j < 40; j++ )
			dc_free(ptr[j]);
		print_statics();
		for(i = 0; i < 40; i++)
			ptr[i] = dc_alloc(POW2(count));
		print_statics();
		for(j = 0; j < 40; j++ )
			dc_free(ptr[j]);
		print_statics();
//		wait();
//	}
	print_statics();
}
}
