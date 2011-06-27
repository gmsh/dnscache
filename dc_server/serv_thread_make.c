

/********************************************************************
 * filename:		serv_thread_make.c
 * author: 		synckey
 * version:		v1.0
 * datetime:		2011-06-27 20:17:28
 * description:		create accept threads
 ********************************************************************/
#include "server.h"
#include "dc_mm.h"

void serv(int connfd);

void thread_make_serv(int i)
{
        void*   thread_main_serv(void *); 
	Pthread_create(&serv_thread_tptr[i].thread_tid,
		                NULL, &thread_main_serv, (void *)i);
	       return;
}


void  * thread_main_serv(void * arg)
{
	int connfd;
	socklen_t clilen;
	struct sockaddr *cliaddr;
	socklen_t	addrlen;

	cliaddr = dc_alloc(addrlen);
	printf("thread %d string \n", (int) arg);
	for(;;){
		clilen = addrlen;
		Pthread_mutex_lock(&serv_thread_mutex);
		connfd = accept(serv_listenfd, cliaddr, &clilen);
		Pthread_mutex_unlock(&serv_thread_mutex);
		serv(connfd);
	//	close(connfd);
	}

}

void serv(int connfd)
{	
	printf("%d is serving\n", pthread_self());
	uint32 totallength;
	if( read(connfd, &totallength, sizeof(uint32)) != sizeof(uint32)){
		close(connfd);
		return;
	}
	if(totallength == 0)
		close(connfd);

	uint8 *requestptr = (uint8 *)dc_alloc(99*sizeof(uint8) + 1);
	* requestptr = totallength;
	int count =TOTAL_LENGTH;
	int total= 0;
	while((count = read(connfd, requestptr+TOTAL_LENGTH + count , 100*sizeof(uint8))) > 0)
		total += count;
	printf("%d bytes reserved\n",total);
	requestptr[total] = 0;
	printf("%s", requestptr+4);



}
/***************  END OF serv_thread_make.c  **************/
