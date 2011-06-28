

/********************************************************************
 * filename:		serv_thread_make.c
 * author: 		synckey
 * version:		v1.0
 * datetime:		2011-06-27 20:17:28
 * description:		create accept threads
 ********************************************************************/
#include "server.h"
#include "dc_mm.h"
#include "dl_cache_stub.h"
void serv(int connfd);

void thread_make_serv(int i)
{
        void*   thread_main_serv(void *); 
	Pthread_create(&serv_thread_tptr[i].thread_tid,
		                NULL, &thread_main_serv, (void *)i);
	       return;
}


void  * thread_main_serv(void * arg)
{
	int connfd;
	socklen_t clilen;
	struct sockaddr *cliaddr;
	socklen_t	addrlen;

	cliaddr = dc_alloc(addrlen);
	printf("thread %d string \n", (int) arg);
	for(;;){
		clilen = addrlen;
		Pthread_mutex_lock(&serv_thread_mutex);
		connfd = accept(serv_listenfd, cliaddr, &clilen);
		Pthread_mutex_unlock(&serv_thread_mutex);
		serv(connfd);
	//	close(connfd);
	}

}

void serv(int connfd)
{	
	printf("%d is serving\n", pthread_self());
	uint32 total_length, magic_number, request_number;
	uint8 reserved_byte;
	uint32 buf_length = TOTAL_LENGTH + MAGIC_NUMBER_LENGTH +
		REQUEST_NUMBER_LENGTH +RESERVED_BYTE_LENGTH;
	char	buf[buf_length];
	if( read(connfd, buf, buf_length*sizeof(uint8)) != buf_length*sizeof(uint8)){
		close(connfd);
		return;
	}
	total_length = *((uint32 *)buf);
	magic_number = *((uint32 *)(buf + TOTAL_LENGTH));
	int i;
	
	request_number	= *((uint32 *)(buf + TOTAL_LENGTH +  MAGIC_NUMBER_LENGTH));
	reserved_byte	= *((uint32 *)(buf + TOTAL_LENGTH +  MAGIC_NUMBER_LENGTH + 
					REQUEST_NUMBER_LENGTH ));
	if(magic_number != MAGIC_NUMBER){
		printf("MAGIC_NUMBER NOT MACHED \n");
		close(connfd);
		return;
	}
	if(total_length == 0)
		close(connfd);

	


	//printf("totallength :%d\n magicnumber: %x\n request_number:%d\nreservedbyte: %c\n",
	//total_length, magic_number,request_number, reserved_byte);


/*	uint8 *requestptr = (uint8 *)dc_alloc(totallength*sizeof(uint8) + 1);
	* requestptr = total_length;
	int count = 0;
	int total= TOTAL_LENGTH;

	while( (count = read(connfd, requestptr + TOTAL_LENGTH +  count ,
			100*sizeof(uint8))) > 0){
		total += count;
		if(total == totallength)
			break;
	}
	printf("%d bytes reserved\n",total);
	requestptr[total] = 0;

*/
}
/***************  END OF serv_thread_make.c  **************/
