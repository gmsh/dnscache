

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
	
	
	uint32 total_length=13;
        uint8 *buf = (uint8 *)malloc(total_length*sizeof(uint8));
	uint32 magic_number = MAGIC_NUMBER;
	uint32 request_number= 1;
	uint8  reserved_byte = 'F';

	*((uint32 *)buf) = total_length;
	*((uint32 *)(buf + TOTAL_LENGTH)) = magic_number;
	*((uint32 *)(buf + TOTAL_LENGTH + MAGIC_NUMBER_LENGTH)) = request_number;
	*((uint8 *)(buf + TOTAL_LENGTH + MAGIC_NUMBER_LENGTH +
		REQUEST_NUMBER_LENGTH))=reserved_byte;

	write(sockfd, buf, total_length);

        //read(sockfd, buf, 100);
        close(sockfd);

}
/***************  END OF servtest.c  **************/
