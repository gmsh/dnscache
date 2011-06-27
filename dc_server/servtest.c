

/********************************************************************
 * filename:		servtest.c
 * author: 		synckey
 * version:		v1.0
 * datetime:		2011-06-27 21:37:11
 * description:		test the serv_thread_make.c
 ********************************************************************/
#include "server.h"

int
main()
{
        int sockfd;
        struct sockaddr_in servaddr;
        sockfd = socket(AF_INET, SOCK_STREAM, 0); 
        bzero(&servaddr, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_port  = htons(PORT);
        inet_pton(AF_INET, "121.250.222.111", &servaddr.sin_addr);
        if( connect(sockfd, (struct sockaddr *)&servaddr, 
                sizeof(servaddr)) != 0)
                printf("connect failed\n");
        else    printf("conneted \n");
	
	
        uint8 *buf = (uint8 *)malloc(10*sizeof(uint8));
	uint32 totallength=10;
	*buf = totallength;
	*(buf+4) = 'a';
	*(buf+5) = 'b';
	*(buf+6) = 'c';
	*(buf+7) = 'd';
	*(buf+8) = 'e';
	*(buf+9) = 'f';
	write(sockfd, buf, 10);

        //read(sockfd, buf, 100);
        close(sockfd);

}
/***************  END OF servtest.c  **************/
