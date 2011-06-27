

/********************************************************************
 * filename:		thread_make.c
 * author: 		synckey
 * version:		v1.0
 * datetime:		2011-06-26 00:45:40
 * description:		make threads
 ********************************************************************/

#include "server.h"

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
	printf("thread %d starting \n", (int)arg );

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
		
		if(++iget == MAXDNSTHREADS)
			iget = 0;		

		Pthread_mutex_unlock(&dns_array_mutex);
	


		
		//do our job 
		dnsrequest(domain, ipptr + count);
		free(domain);	//free the memory of domain
	
		Pthread_mutex_lock(mutex);
		(*number)--;
		Pthread_mutex_unlock(mutex);

		if((*number) == 0){
			write(connfd, ipptr, total*sizeof(uint32));
		//	close(connfd);
			free(ipptr);
			free(number);
			free(mutex);
		} 


	}

}
/***************  END OF thread_make.c  **************/
