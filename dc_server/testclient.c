

/********************************************************************
 * filename:		testclient.c
 * author: 		synckey
 * version:		v1.0
 * datetime:		2011-06-26 20:33:20
 * description:		test the server
 ********************************************************************/
#include "server.h"
int main()
{
	
	int sockfd;
	struct sockaddr_in servaddr;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port  = htons(9999);
	inet_pton(AF_INET, "localhost", &servaddr.sin_addr);
	if( connect(sockfd, (struct sockaddr *)&servaddr, 
		sizeof(servaddr)) != 0)
		printf("connect failed\n");
	else 	printf("conneted \n");

	uint32 buf[100];
	read(sockfd, buf, 100);
	char ipstr[16];
	int i;
	for(i = 0 ; i < 16 ;i++)
		ipstr[i] = '\0';
	for(i = 0; i<5; i++){
		inet_ntop(AF_INET, (in_addr_t *)(buf+i), ipstr , 16);
		printf("%s\n", ipstr);
	}
	close(sockfd);
	

	
}

/***************  END OF testclient.c  **************/
