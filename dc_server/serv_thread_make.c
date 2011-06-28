

/********************************************************************
 * filename:		serv_thread_make.c
 * author: 		synckey
 * version:		v1.0
 * datetime:		2011-06-27 20:17:28
 * description:		create accept threads and some methods
 ********************************************************************/
#include "server.h"
#include "dc_mm.h"
//#include "dl_cache_stub.h"
void serv(int connfd);
void do_search(char *headptr, char *requestptr, int rbuflen ,int conndfd);

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
	int readcount;
	int n;
	uint32 total_length, magic_number, request_number;
	uint8 reserved_byte;
	uint32 buf_length = TOTAL_LENGTH + MAGIC_NUMBER_LENGTH +
		REQUEST_NUMBER_LENGTH +RESERVED_BYTE_LENGTH;
	char	*buf =(char *)dc_alloc(buf_length*sizeof(uint8)); /*head bufptr*/

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
	printf(" %d , %c, %d, %d\n", total_length, reserved_byte,
	request_number, magic_number);
	
	if(total_length == 0)
		close(connfd);
	
	char *request_ptr =(char *)dc_alloc((total_length - buf_length ) * sizeof(uint8));
	char *read_ptr = request_ptr;
	readcount = buf_length;	/* head length */
	while( (i = read(connfd , read_ptr, (total_length - buf_length))) > 0){
		readcount += i;
		read_ptr += i;
		if((readcount == total_length) || (readcount > total_length)){
			*(request_ptr + total_length) = '\0';
			break;	
		}
	}
	do_search(buf, request_ptr, total_length - buf_length ,connfd);
}

/* search the cache ,and build the return message */

void do_search(char *headptr, char *requestptr, int rbuflen, int connfd)
{	
	char *currentptr = requestptr;
	int readcount ;
	for(;;){
	readcount += strlen(currentptr) + 1;
	printf("%s\n", currentptr);
	if(rbuflen == readcount )
			break;
	}

	return;
}

/***************  END OF serv_thread_make.c  **************/
