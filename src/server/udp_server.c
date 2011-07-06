

/********************************************************************
 * filename:		udp_server.c
 * author: 		synckey
 * version:		v1.0
 * datetime:		2011-07-02 12:55:56
 * description:		the udp server
 ********************************************************************/

#include "server.h"

uint32 dns_error , cache_miss;

static void udp_recv(int sockfd, struct sockaddr *cliaddr, socklen_t clilen);
static void do_search(int sockfd, uint8 *buf, struct sockaddr * cliaddr,
			socklen_t  *len);
static void do_dns_search(dm_node * firstnode);

void  start_udp_server(void )
{
	
	inet_pton(AF_INET, DNS_ERROR, &dns_error);
	inet_pton(AF_INET, CACHE_MISS, &cache_miss);

	int sockfd;
	int n;
	socklen_t clilen;
	struct sockaddr_in servaddr ,cliaddr;
	
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(PORT);

	inet_pton(AF_INET, SERVIP, &servaddr.sin_addr);
	if( -1 == bind(sockfd, (struct sockaddr *)&servaddr,
		sizeof(servaddr))){
		perror("udp bind");
		exit(0);
		
	}
	n = UDP_BUF_LENGTH;
	setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &n, sizeof(n));
	printf("udp server started\n");
	udp_recv(sockfd, (struct sockaddr *)&cliaddr, sizeof(cliaddr));

}

static void udp_recv(int sockfd, struct sockaddr *cliaddr, socklen_t clilen)
{		
	int8  *readbuf =(int8 *)dc_alloc((HEAD_LENGTH + MAX_UDP_LENGTH) * sizeof(uint8)); 
	int n;
	socklen_t *len;
	len =(socklen_t *) dc_alloc(sizeof(socklen_t));
	for( ; ; ){
		*len = clilen;
		n = recvfrom(sockfd, readbuf, HEAD_LENGTH + MAX_UDP_LENGTH,
				0, cliaddr, len);

		if(-1 == n){
			perror("recvfrom");
			exit(0);
		}
#ifdef DEBUG
		printf("receive udp request\n");
#endif	
		if( MAGIC_NUMBER == *(uint32 *)(readbuf + TOTAL_LENGTH))
			do_search(sockfd, readbuf, cliaddr, len);
	}

}

static void do_search(int sockfd, uint8 *buf,
		struct sockaddr * cliaddr, socklen_t  *len)
{	
	int n;
	uint32 total_length;
	total_length = *((uint32 *)buf);
	if( 0 == total_length ){
		return;
	}

	uint8 *currentptr =(uint8 *)(buf + HEAD_LENGTH);

	int readcount = 0, misscount = 0;
	uint32 *ipptr = (uint32 *)currentptr;
	data_t *result;
	uint32 ipcount = 0;
	//uint32 *tempptr;
	dm_node *curr;
	dm_node *firstnode ;
	

	for(;;){
		result = (data_t *)kache_retrieve(currentptr);
		if( (NULL == result)|| (result -> timestamp + TIME_OUT < time(NULL)) ||
			result -> ip == cache_miss /*|| result -> ip == dns_error*/){
			if(0 == misscount){
				curr = (dm_node *)dc_alloc(sizeof(dm_node));
				firstnode = curr;
			} else{
				curr -> next = (dm_node*)dc_alloc(sizeof(dm_node));
				curr = curr -> next;
			}
				
			curr -> next = NULL;

			misscount++;

#ifdef DEBUG
			printf("misscount %d\n", misscount);
#endif
			
			curr -> domain = (uint8 *) dc_alloc((strlen(currentptr) + 1) 
					* sizeof(uint8));
                        strcpy(curr -> domain, currentptr);

			readcount += strlen(currentptr) + 1;
			currentptr  += strlen(currentptr) + 1;	
			inet_pton(AF_INET,  CACHE_MISS, ipptr + ipcount);

		} else {
		//	tempptr = currentptr;
			readcount += strlen(currentptr) + 1;
			currentptr  += strlen(currentptr) + 1;	
			*(ipptr + ipcount) = result -> ip;

		}
		
		ipcount++;
		if(readcount >= total_length - HEAD_LENGTH)
			break;

	}
	
	if( 0 == misscount)
		*((uint8 *)(buf + HEAD_LENGTH - 1 )) = FIRST_WITHOUT_ERROR;
	else
		*((uint8 *)(buf + HEAD_LENGTH - 1 )) = FIRST_WITH_ERROR;
	
	ssize_t send_len;
	send_len = HEAD_LENGTH + ipcount * sizeof(uint32); 
	if(send_len != (sendto(sockfd, buf, send_len, 0, cliaddr, *len))){
		perror("sendto");
		return;
	}

#ifdef DEBUG
	printf("sended\n");
#endif
	if(0 != misscount)
		do_dns_search(firstnode);
	return;
}

static void do_dns_search(dm_node * firstnode)
{
	dm_node *currnode = firstnode, *tempnode;

	while(NULL != currnode){
#ifdef DEBUG
		printf("a domian miss, udp dns request\n");
#endif 
		Pthread_mutex_lock(&dns_array_mutex);
		dns_array[iput].domain 	= currnode -> domain;
                dns_array[iput].index  	= -1;
                dns_array[iput].sockfd 	= -1;
                dns_array[iput].number 	= NULL;
                dns_array[iput].count  	= -1;
                dns_array[iput].total  	= -1;
                dns_array[iput].ipptr  	= NULL;
                dns_array[iput].mutex  	= NULL;
                dns_array[iput].headptr	= NULL;
                dns_array[iput].miss	= NULL;
                dns_array[iput].tcp_or_udp	= UDP_DNS_REQUEST;
		
		if(++iput == ARRAYSIZE)
                           iput = 0;
                if( iget == iput ){
                 	printf("iget == iput\n");
		 	exit(0);
		}
                Pthread_cond_signal(&dns_array_cond);
               	Pthread_mutex_unlock(&dns_array_mutex);
		tempnode = currnode;
		currnode = currnode -> next;
		dc_free(tempnode);
	}
	return;
}
/***************  END OF serv_thread_make.c  **************/
