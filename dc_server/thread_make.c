

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
	void	*thread_main_dns(void *);
	if(pthread_create(&dns_thread_tptr[i].thread.tid,
		NULL, &thread_main_dns, (void *)i) != 0){
		printf("can not create dns thread %d\n", i);
		exit(1);
	}
	return;

	

}


void *
thread_main_dns(void *arg)
{
	int dnsrequest(const char *name ,uint32 *ipaddr);
	
	printf("thread %d starting \n",(int)arg );
	for(;;){
		if(Pthread_mutex_lock(&dns_array_mutex) !=0 ){
			printf("can not get lock\n");
			exit(1);
		}
		while(iget == iput){
			if(pthread_cond_wait(&dns_array_cond,
					&dns_array_mutex) != 0){
				printf("can not wait \n");
				exit(1);			
			}
		}
		dnsrequest(dns_array[iget].domain,dns_array[iget].ipptr
			+dns_array[iget]+count);
		free(domain);			//
		if(pthread_mutex_lock(dns_array[iget].mutex) !=0 ){
			printf("can not get lock\n");
			exit(1);
		}
		if(--dns_array[iget].number == 0){
			int connfd = sockfd;
			write(connfd, ipptr, 
				dns_array[iget].total*sizeof(uint32));
			close(connfd);
			free(ipptr);

		}
		if(pthread_mutex_unlock(dns_array[iget].mutex) !=0 ){
			printf("can not unlock\n");
			exit(1);
		}
		if(++iget == MAXTHREADS)
			iget = 0;		
		if(pthread_mutex_unlock(&dns_array_mutex) !=0 ){
			printf("can not unlock\n");
			exit(1);
		}



	}

}
/***************  END OF thread_make.c  **************/
