

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
		printf("udp bind error\n");	
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
		printf("udp rec\n");
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
	fake_data_t *result;
	uint32 ipcount = 0;
	uint32 *tempptr;

	for(;;){
		result = (fake_data_t *)get_data_and_lock(currentptr);
		if( (NULL == result)|| (result -> timestamp + TIME_OUT < time(NULL)) ||
			result -> ip == cache_miss || result -> ip == dns_error){

			unlock_after_copy(currentptr);
			misscount++;
			
			printf("misscount %d\n", misscount);
			
			readcount += strlen(currentptr) + 1;
			currentptr  += strlen(currentptr) + 1;	
			inet_pton(AF_INET,  CACHE_MISS, ipptr + ipcount);

		} else {
			tempptr = currentptr;
			readcount += strlen(currentptr) + 1;
			currentptr  += strlen(currentptr) + 1;	

			*(ipptr + ipcount) = result -> ip;
			unlock_after_copy(tempptr);
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
	printf("sended\n");

	return;
}

 
/***************  END OF serv_thread_make.c  **************/
