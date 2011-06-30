#include "dc_mm.h"

main(){
	mm_init();
	void * ptr[10];
	int i, j;
	for(i = 0; i < 10; i++)
		ptr[i] = dc_alloc(256);
	for(j = 0; j < 10; j++ )
			dc_free(ptr[j]);
	ptr[i] = dc_alloc(256);
	dc_free(ptr[i]);
}
