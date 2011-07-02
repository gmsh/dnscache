/********************************************************************
 * filename:		dc_client.c
 * author: 		synckey
 * version:		v1.0
 * datetime:		2011-07-01 20:02:10
 * description:		headfile for clientrequest.c
 ********************************************************************/
#ifndef DC_CLIENT_H
#define DC_CLIENT_H 1

#include "typedefs.h"
#include "constants.h"
#include <sys/uio.h>

#define SUCCESS         0
#define NOT_IN_SET      1
#define CONNECT_WRONG   2
#define OTHER_WRONG     3

/*
 *  return 0 success,
 *         1 there is character in buf not in the code set
 *         2 if connect wrong
 *         3 other wrong
 *         f is the fuction you want do with each pair of <domain ,ip>
 */    
int  dns_search(char *buf, int nbuf, void (*f)(char *,uint32 *));


#endif /*DC_CLIENT_H*/

/***************  END OF clientrequst.h  **************/
