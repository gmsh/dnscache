

/********************************************************************
 * filename:		mainserver.h
 * author: 		synckey
 * version:		v1.0
 * datetime:		2011-06-27 15:51:59
 * description:		main method
 ********************************************************************/

#include "server.h"
//#include "dc_mm.h"
int
main(int argc, char **argv)
{
	int 	i;
	socklen_t addrlen;
	struct sockaddr_in servaddr;
	
	serv_listenfd = socket(AF_INET, SOCK_STREAM, 0);
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port   = htons(PORT);
	if(bind(serv_listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr) ) != 0){	
		printf("bind error\n");
		exit(0);
	}
	
	serv_thread_tptr = calloc(MAXSERVTHREADS, sizeof(thread_t));
	
	int backlog;
	char *ptr;
	if( (ptr = getenv("LISTENQ")) != NULL )
		backlog = atoi(ptr);
	else 
		backlog = MAXLISTENQ ;
	
	listen(serv_listenfd, backlog);

	for(i = 0; i < MAXSERVTHREADS; i++ ){
		thread_make_serv(i);
	}
	

	for(;;)
		pause();

}


/***************  END OF mainserver.h  **************/
