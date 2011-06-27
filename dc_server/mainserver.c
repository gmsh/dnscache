

/********************************************************************
 * filename:		mainserver.h
 * author: 		synckey
 * version:		v1.0
 * datetime:		2011-06-27 15:51:59
 * description:		main method
 ********************************************************************/

#include "server.h"

int
main(int argc, char **argv)
{
	int 	i, listenfd, connfd;
	socklen_t addrlen, clilen;
	struct sockaddr_in cliaddr, servaddr;
	
	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port   = htons(PORT);
	if(bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr) )!= 0){	
		printf("bind error\n");
		exit(0);
	}
	listen(listenfd, 5);

	for()

}


/***************  END OF mainserver.h  **************/
