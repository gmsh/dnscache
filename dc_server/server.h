  

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

#include "constants.h"
#include "typedefs.h"
//#include "dc_mm.h"
//#include "dl_cache_stub.h"
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
#include <errno.h>
#include <fcntl.h>

#define MAXLINE		4096
#define ARRAYSIZE	1024000
#define  LINES      	100000


#define  DNS_ERROR	"127.0.0.1"
#define	 CACHE_MISS	"127.0.0.2"
#define  FIRST_WITH_ERROR	0x3f	//0011 1111  0 with error , 
					//	     1 without error
#define  FIRST_WITHOUT_ERROR 	0x7f	//0111 1111
#define	 SECOND_WITH_ERROR	0xbf	//1011 1111
#define	 SECOND_WITHOUT_ERROR	0xff	//1111 1111

#define	 HAVE_ERROR	0	
#define	 NO_ERROR	1	

typedef struct dmnode{
	char	*domain;	/* point to the domain */
	uint32  index;		/* the index in the first message */
	struct dmnode	*next;	/* point to the next node */
} dm_node;


typedef struct{
	pthread_t thread_tid;	/* thread ID */
//	unsigned long	thread_count;	/* works handled, just for test */
} thread_t;


/*
 *the struct of the array ,the dns threads will read it
 *and know how to server .
 */
typedef struct{
	int sockfd;		/* the connected fd */
	int *number;		/* domains to search reduce 1 when one dns
				 * search is finished.		
	 			 */
	uint32 index;
	int count;		/* the order of the request*/
	int total;		/* total domians */
	char *domain;		/* point to the domain */
	uint32 *ipptr;		/* first address to write ip */
	pthread_mutex_t	*mutex;	/* lock the number */
	uint8	*headptr;	/* point to the message head */
	uint8	*miss;		/* if there is dns error in the second message
				 * 0 if there is , 1 if	all is right
				 */
} dns_thread_t;

thread_t *dns_thread_tptr;	/* point to the memory alloc for 
				   dns threads	
				 */
thread_t *serv_thread_tptr;	/* point to the memory alloc for 
				   connect threads	
				 */
dns_thread_t dns_array[ARRAYSIZE];		
				/*
				 *  dns threads read it and know
				 *  how to server
				 */

pthread_mutex_t dns_array_mutex;/*
				 *  lock the dns_array;
				 */

pthread_cond_t	dns_array_cond; /*
				 *  notify the dns threads;
				 */
pthread_mutex_t serv_thread_mutex;
				/*
				 * server threads'  lock
				 */

int iget, iput;			/* 
				 * iget, next idex to put in dns array,
				 * iput, next to read 
				 */




int serv_listenfd;
void thread_make_dns(int i);
void *thread_main_dns(void *arg);
void thread_make_serv(int i);
void *thread_main_serv(void * arg);
void init(void);

/* wraped functions */
void	Pthread_create(pthread_t *, const pthread_attr_t *,
					   void * (*)(void *), void *);
void	Pthread_join(pthread_t, void **);
void	Pthread_detach(pthread_t);
void	Pthread_kill(pthread_t, int);

void	Pthread_mutexattr_init(pthread_mutexattr_t *);
void	Pthread_mutexattr_setpshared(pthread_mutexattr_t *, int);
void	Pthread_mutex_init(pthread_mutex_t *, pthread_mutexattr_t *);
void	Pthread_mutex_lock(pthread_mutex_t *);
void	Pthread_mutex_unlock(pthread_mutex_t *);

void	Pthread_cond_broadcast(pthread_cond_t *);
void	Pthread_cond_signal(pthread_cond_t *);
void	Pthread_cond_wait(pthread_cond_t *, pthread_mutex_t *);
void	Pthread_cond_timedwait(pthread_cond_t *, pthread_mutex_t *,
							   const struct timespec *);

void	Pthread_key_create(pthread_key_t *, void (*)(void *));
void	Pthread_setspecific(pthread_key_t, const void *);
void	Pthread_once(pthread_once_t *, void (*)(void));

void	 err_dump(const char *, ...);
void	 err_msg(const char *, ...);
void	 err_quit(const char *, ...);
void	 err_ret(const char *, ...);
void	 err_sys(const char *, ...);


#endif /*SERVER_H_*/
/***************  END OF server.h  **************/
