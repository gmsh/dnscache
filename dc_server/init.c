

/********************************************************************
 * filename:		init.c
 * author: 		synckey
 * version:		v1.0
 * datetime:		2011-06-28 10:49:16
 * description:		must be called befor do other operations.
 ********************************************************************/
#include "server.h"
#include "dl_cache_stub.h"









void init()
{	
	//init the dl_cache
	dl_cache_init(sizeof(fake_data_t));	
}	

/***************  END OF init.c  **************/
