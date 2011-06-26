

/**********************************************************
 * filename:		server.h
 * author: 		synckey
 * version:		v1.0
 * datetime:		2011-06-25 22:13:32
 * description:		Some head file that are needed for
 			network program.
 **********************************************************/

#ifndef SERVER_H_
#define SERVER_H_ 1

#include "typedefs.h"
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <pthread.h>

#define	MAXDNSTHREADS	512	/* the number of DNS threads  */
#define	MAXSERVTHREADS	4096	/* the number of work threads */

typedef struct{
	pthread_t thread_tid;	/* thread ID */
	unsigned long	thread_count;	/* works handled, just for test */
} thread_t;


/*
 *the struct of the array ,the dns threads will read it
 *and know how to server .
 */
typedef struct{
	int sockfd;		/* the connected fd */
	char *domain;		/* point to the domain */
	uint32 number;		/* domains to search */
	pthread_mutex_t	*mutex;	/* lock the number */
} dns_thread_t;












#endif /*SERVER_H_*/
/***************  END OF server.h  **************/
