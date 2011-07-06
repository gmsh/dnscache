

/********************************************************************
 * filename:		init.c
 * author: 		synckey
 * version:		v1.0
 * datetime:		2011-06-28 10:49:16
 * description:		must be called befor do other operations.
 ********************************************************************/

#include "server.h"




void init()
{	
	mm_init();
	
	//init the cache
	kache_init();	
		
}	

/***************  END OF init.c  **************/
