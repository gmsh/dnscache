

/********************************************************************
 * filename:		test.c
 * author: 		synckey
 * version:		v1.0
 * datetime:		2011-06-25 22:17:25
 * description:		test the functions 
 ********************************************************************/

#include "server.h"
#include "dc_mm.h"

int dnsrequest(char *, uint32 *);
pthread_mutex_t	dns_array_mutex= PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t	dns_array_cond= PTHREAD_COND_INITIALIZER;

int main(int argc, char **argv)
{	

	/*test the thread_make()*/

	int i, listenfd, connfd;
	void	thread_make_dns(int);
	socklen_t addrlen, clilen;
	struct sockaddr_in cliaddr, servaddr;
	int  *number, count, total;
	char *cptr[5];
	uint32 *ipptr;
	pthread_mutex_t *mutex;
	
	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port   = htons(9999);
	if(bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr) )!= 0){	
		printf("bind error\n");
		exit(0);
	}
	listen(listenfd, 5);

	dns_thread_tptr = calloc(MAXDNSTHREADS, sizeof(dns_thread_t));
	iget = iput = 0;
	for(i = 0; i < 20; i ++ )
		thread_make_dns(i);

	for(;;){
		clilen = sizeof(cliaddr);
		connfd = accept(listenfd, (struct sockaddr *)&cliaddr,
				&clilen);
		printf("client connected\n");
		number= (int *)dc_alloc(sizeof(int)); 
		*number = 5;
		ipptr = (uint32 *)dc_alloc(5*sizeof(uint32));
		mutex = (pthread_mutex_t *)dc_alloc(sizeof(pthread_mutex_t));
		Pthread_mutex_init(mutex, NULL);
		
		for(i=0; i < 5 ; i++){
			cptr[i] = (char *)dc_alloc(64*sizeof(char));
			strcpy(cptr[i],"google.com");
			Pthread_mutex_lock(&dns_array_mutex);
//		while((iget!=0)&&((iget + 1) % MAXDNSTHREADS == iput))
//			sleep(0.02);
			
			dns_array[iput].domain = cptr[i];
			dns_array[iput].sockfd = connfd;
			dns_array[iput].number = number;
			dns_array[iput].count  = i;
			dns_array[iput].total  = 5;
			dns_array[iput].ipptr  = ipptr;
			dns_array[iput].mutex  = mutex;
			if(++iput == MAXDNSTHREADS)
				iput = 0;
			if(iget == iput )
				exit(0);

			Pthread_cond_signal(&dns_array_cond);
			Pthread_mutex_unlock(&dns_array_mutex);
	
		}

	}

	/*test the function dnsrequest */
	/*
	uint32 addr;
	char ipstr[16];
	int i;
	for(i = 0 ; i < 16 ;i++)
		ipstr[i] = '\0';
	int a = dnsrequest("www.931momo.cn", &addr);
	inet_ntop(AF_INET, (in_addr_t *)&addr, ipstr, 16);
	printf("%s\n", ipstr);
	if( a != 0)
		printf("%s\n", gai_strerror(a));
	return(0);
	*/
}

/***************  END OF test.c  **************/
