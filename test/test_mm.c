#include "dc_mm.h"

main(){
	mm_init();
	void * ptr[18];
	int i, j;
	for(i = 0; i < 18; i++)
		ptr[i] = dc_alloc(1048576);
	for(j = 0; j < 18; j++ )
			dc_free(ptr[j]);
	ptr[i] = dc_alloc(256);
	dc_free(ptr[i]);
}
