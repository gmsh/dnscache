

/********************************************************************
 * filename:		thread_make.c
 * author: 		synckey
 * version:		v1.0
 * datetime:		2011-06-26 00:45:40
 * description:		make threads
 ********************************************************************/

#include "server.h"


static  void*	thread_main_dns(void *);

void thread_make_dns(int i)
{
	Pthread_create(&dns_thread_tptr[i].thread_tid,
		NULL, &thread_main_dns, (void *)i);
	return;
}

static void *
thread_main_dns(void *arg)
{	
	struct iovec iovec[2]; //for writev
	int connfd, *number, count, total;
	char  *domain ;
	uint32 *ipptr, index;
	uint8 *headptr, *miss;
	pthread_mutex_t *mutex;
	uint8 missindex = NO_ERROR;
	uint8 tcp_or_udp = 0;
#ifdef DEBUG 
	printf("dns thread %u starting \n", (int)arg );
#endif
	for(;;){
		
		Pthread_mutex_lock(&dns_array_mutex);
		while(iget == iput)
			Pthread_cond_wait(&dns_array_cond,
					&dns_array_mutex);
#ifdef DEBUG	
		printf("dns thread %u  working \n", pthread_self());
#endif
		connfd	= dns_array[iget].sockfd;
		domain  = dns_array[iget].domain;
		number	= dns_array[iget].number;
 		count	= dns_array[iget].count;
		total	= dns_array[iget].total;
		ipptr	= dns_array[iget].ipptr;
		mutex	= dns_array[iget].mutex;
		index	= dns_array[iget].index;
		headptr	= dns_array[iget].headptr;
		miss	= dns_array[iget].miss;
		tcp_or_udp = dns_array[iget].tcp_or_udp;
		if(++iget == ARRAYSIZE)
			iget = 0;		

		Pthread_mutex_unlock(&dns_array_mutex);
		
		//udp dns request
		if( UDP_DNS_REQUEST == tcp_or_udp){
			uint32 ip;
			data_t data;
			if(dnsrequest(domain, &ip) != 0)
				inet_pton(AF_INET,  DNS_ERROR, &ip);
			data.ip = ip;
			data.timestamp = time(NULL);

			kache_store(domain, &data);
#ifdef DEBUG
			printf("%s dns result writed \n", domain);
#endif
			dc_free(domain);	//dc_free the memory of domain
			
		} else{
			//tcp dns request
			*(uint32 *)(ipptr + count * 2) = index;
			if(dnsrequest(domain, ipptr + count * 2 + 1  ) != 0){
				inet_pton(AF_INET,  DNS_ERROR, ipptr + count * 2 + 1);
				missindex = HAVE_ERROR;
			}
			data_t data;
			data.ip = *(uint32 *)(ipptr + count *2 + 1 );
			data.timestamp = time(NULL);
			kache_store(domain, &data);

#ifdef DEBUG	
			printf(" tcp index %d  finished \n", *(ipptr + count * 2) );
			printf(" domain %s \n", domain );
#endif
			dc_free(domain);	//dc_free the memory of domain
			Pthread_mutex_lock(mutex);
			if(HAVE_ERROR == missindex)
				*miss = HAVE_ERROR;
			(*number)--;
			Pthread_mutex_unlock(mutex);
			if((*number) == 0){
				if(HAVE_ERROR == *miss)
					*((uint8 *)(headptr + HEAD_LENGTH - 1 )) 
						= SECOND_WITH_ERROR;
				else 
					*((uint8 *)(headptr + HEAD_LENGTH - 1 )) =
						SECOND_WITHOUT_ERROR;
			
				iovec[0].iov_base = headptr ;
				iovec[0].iov_len  = HEAD_LENGTH;
				iovec[1].iov_base = ipptr ;
				iovec[1].iov_len  = total * 2 * sizeof(uint32);
				
				if(writev(connfd, iovec, 2) != 
					HEAD_LENGTH + total * 2 * sizeof(uint32)){
					perror("writev");
					exit(0);
				}	
#ifdef DEBUG	
				printf("seconde return sent\n");
#endif
				close(connfd);
				dc_free(ipptr);
				dc_free(number);
				dc_free(mutex);
				dc_free(headptr);
				dc_free(miss);
			}
		}
	}

}
/***************  END OF thread_make.c  **************/
