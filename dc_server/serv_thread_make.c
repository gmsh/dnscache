

/********************************************************************
 * filename:		serv_thread_make.c
 * author: 		synckey
 * version:		v1.0
 * datetime:		2011-06-27 20:17:28
 * description:		create accept threads and some methods
 ********************************************************************/
#include "server.h"
//#include "dc_mm.h"
#include "dl_cache_stub.h"
static void serv(int connfd);
static void do_search(uint8* headptr, uint8 *requestptr, int rbuflen ,int conndfd);
static void do_dns_search(uint8 * headptr,int total, dm_node * firstnode, int connfd);

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

static void serv(int connfd)
{	
	printf("%d is serving\n", pthread_self());
	int readcount;
	int n;
	uint32 total_length, magic_number, request_number;
	uint8 reserved_byte;
	
	char	*buf =(char *)dc_alloc(HEAD_LENGTH*sizeof(uint8)); /*head bufptr*/

	if( read(connfd, buf, HEAD_LENGTH*sizeof(uint8)) != HEAD_LENGTH*sizeof(uint8)){
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
	printf("%d , %c, %d, %d\n", total_length, reserved_byte,
	request_number, magic_number);
	
	if(total_length == 0)
		close(connfd);
	
	uint8 *request_ptr =(uint8 *)dc_alloc((total_length - HEAD_LENGTH) * sizeof(uint8));
	uint8 *read_ptr = request_ptr ;
	
	readcount = HEAD_LENGTH;	/* head length */
	while( (i = read(connfd , read_ptr, (total_length - HEAD_LENGTH))) > 0){
		readcount += i;
		read_ptr += i;
		if(readcount >= total_length){
			*(request_ptr + total_length - HEAD_LENGTH - 1) = '\0';
			break;	
		}
	}
	do_search(buf, request_ptr, total_length - HEAD_LENGTH ,connfd);
}

/* search the cache ,and build the return message,
 * rbuflen  means the length of the message(except headlength)
 */

static void do_search(uint8 * headptr, uint8 *requestptr, int rbuflen, int connfd)
{	
	uint8 *currentptr = requestptr;
	int readcount = 0, misscount = 0;
	dm_node *curr = dc_alloc(sizeof(dm_node));
	dm_node *firstnode = curr;
	uint32 *ipptr = (uint32 *)requestptr;
	fake_data_t *result;
	uint32 ipcount = 0;

	for(;;){
		
		result = (fake_data_t *)get_data_and_lock(currentptr);
		if( (NULL == result) || (result -> timestamp + 
					TIME_OUT < time(NULL)) ){
			unlock_after_copy(currentptr);
			misscount++;
			curr -> domain = (uint8 *) dc_alloc((strlen(currentptr) + 1) 
						* sizeof(uint8));
			curr -> index = ipcount;
			strcpy(curr -> domain, currentptr);
			readcount += strlen(currentptr) + 1;
			currentptr  += strlen(currentptr) + 1;	
			inet_pton(AF_INET,  CACHE_MISS, ipptr + ipcount);
			/*CACHE_MISS 127.0.0.2*/

		} else {
			*(ipptr + ipcount) = result -> ip;
			unlock_after_copy(currentptr);
			readcount += strlen(currentptr) + 1;
			currentptr  += strlen(currentptr) + 1;	
		}
		
		ipcount++;
		if(readcount >= rbuflen)
			break;
		if( NULL == result ){
			curr -> next = (dm_node *)dc_alloc(sizeof(dm_node));
			curr = curr -> next;
			curr -> next =NULL;
		}

	}

	if( 0 == misscount){
		*((uint8 *)(headptr + HEAD_LENGTH - 1 )) = FIRST_WITHOUT_ERROR;
		write(connfd, headptr, HEAD_LENGTH);
		write(connfd, requestptr, ipcount * sizeof(uint32));
		dc_free(headptr);
		dc_free(requestptr);
		close( connfd );
	
	}else {
		*((uint8 *)(headptr + HEAD_LENGTH - 1 )) = FIRST_WITH_ERROR;
		write(connfd, headptr, HEAD_LENGTH);
		write(connfd, requestptr, ipcount *sizeof(uint32));
		dc_free(requestptr);
		do_dns_search(headptr, misscount, firstnode, connfd);
	}

	return;
}

/*
 * call up the dns threads to do dns search.
 */

static void do_dns_search(uint8* headptr, int total, dm_node * firstnode, int connfd)
{
	dm_node *currnode = firstnode, *tempnode ;
	int i;
	
	int  *number, count;
	uint32 *ipptr, index;
	uint8	*miss = dc_alloc(sizeof(uint8));
	*miss = NO_ERROR;
	pthread_mutex_t *mutex;
	number= (int *)dc_alloc(sizeof(int)); 
	*number = total;
	ipptr = (uint32	*)dc_alloc(total * 2 * sizeof(uint32));
	mutex = (pthread_mutex_t*)dc_alloc(sizeof(pthread_mutex_t));
	Pthread_mutex_init(mutex, NULL);

	i = 0;
	while(NULL != currnode){
		Pthread_mutex_lock(&dns_array_mutex);
		dns_array[iput].domain 	= currnode -> domain;
                dns_array[iput].index  	= currnode -> index;
                dns_array[iput].sockfd 	= connfd;
                dns_array[iput].number 	= number;
                dns_array[iput].count  	= i;
                dns_array[iput].total  	= total;
                dns_array[iput].ipptr  	= ipptr;
                dns_array[iput].mutex  	= mutex;
                dns_array[iput].headptr	= headptr;
                dns_array[iput].miss	= miss;
		if(++iput == ARRAYSIZE)
                           iput = 0;
                if( iget == iput ){
                 	printf("iget == iput\n");
		 	exit(0);
		}

                Pthread_cond_signal(&dns_array_cond);
                Pthread_mutex_unlock(&dns_array_mutex);
		i++;
		tempnode = currnode;
		currnode = currnode -> next;
		dc_free(tempnode);
		
	
	}
}

 
/***************  END OF serv_thread_make.c  **************/
