#include "dc_mm.h"

main(){
	mm_init();
	void * ptr[5];
	int i, j;
	for(i = 0; i < 5; i++)
		ptr[i] = dc_alloc(9000);
	for(j = 0; j < 5; j++ )
			dc_free(ptr[j]);
	ptr[i] = dc_alloc(9000);
	dc_free(ptr[i]);
}
