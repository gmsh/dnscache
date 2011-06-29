

/********************************************************************
 * filename:		thread_make.c
 * author: 		synckey
 * version:		v1.0
 * datetime:		2011-06-26 00:45:40
 * description:		make threads
 ********************************************************************/

#include "server.h"
#include "dc_mm.h"
//#include "dc_mm_structs.h"
void thread_make_dns(int i)
{
	void*	thread_main_dns(void *);
	Pthread_create(&dns_thread_tptr[i].thread_tid,
		NULL, &thread_main_dns, (void *)i);
	return;
}


void *
thread_main_dns(void *arg)
{
	int dnsrequest(const char *name, uint32 *ipaddr);
	int connfd, *number, count, total;
	char *domain ;
	uint32 *ipptr;
	pthread_mutex_t *mutex;
	printf("dns thread %d starting \n", (int)arg );

	for(;;){

		Pthread_mutex_lock(&dns_array_mutex);

		while(iget == iput)
			Pthread_cond_wait(&dns_array_cond,
					&dns_array_mutex);
		
		printf("thread %d  working \n", pthread_self());

		connfd	= dns_array[iget].sockfd;
		domain  = dns_array[iget].domain;
		number	= dns_array[iget].number;
 		count	= dns_array[iget].count;
		total	= dns_array[iget].total;
		ipptr	= dns_array[iget].ipptr;
		mutex	= dns_array[iget].mutex;
		
		if(++iget == ARRAYSIZE)
			iget = 0;		

		Pthread_mutex_unlock(&dns_array_mutex);

		//do our job 
		if(dnsrequest(domain, ipptr + count) != 0) 
			inet_pton(AF_INET, "127.0.0.1", ipptr+count);
		dc_free(domain);	//dc_free the memory of domain
	
		Pthread_mutex_lock(mutex);
		(*number)--;
		Pthread_mutex_unlock(mutex);
		if((*number) == 0){
			printf("nimei a \n");
			write(connfd, ipptr, total*sizeof(uint32));
			printf("sended\n");
	//		close(connfd);	//todo
			dc_free(ipptr);
			dc_free(number);
			dc_free(mutex);
		} 


	}

}
/***************  END OF thread_make.c  **************/
