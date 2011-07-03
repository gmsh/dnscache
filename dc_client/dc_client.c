

/********************************************************************
 * filename:		dc_client.c
 * author: 		synckey
 * version:		v1.0
 * datetime:		2011-07-01 18:40:11
 * description:		client api
 ********************************************************************/
#include "dc_client.h"

uint32 dns_error , cache_miss;

static int	is_in_set(char * c);
static void	build_head(uint8* headbuf, uint32 total_length );
static int	pre_do(uint8* buf, uint32* total_length, uint32* index, int nbuf);
static int	tcp_dns_search(char *buf, int nbuf, void (*f)(char*, uint32 *));
static int	udp_dns_search(char *buf, int nbuf, void (*f)(char*, uint32 *));

/*  chose to use tcp or udp.
 *  return 0 success,
 *	   1 there is character in buf not in the code set
 *	   2 if connect wrong
 *	   3 other wrong
 *	   f is the fuction you want do with each pair of <domain ,ip>
 *	   nbuf number of domains
 */

int  	dns_search(char *buf, int nbuf, void (*f)(char *,uint32 *))
{
	if(nbuf < TCP_OR_UDP )	// chose tcp 
		return tcp_dns_search(buf, nbuf, f);
	else 			// udp
		return udp_dns_search(buf, nbuf, f);
}

/*  do dns search with tcp connection.
 *  return SUCCESS  success,
 *	   NOT_IN_SET there is character in buf not in the code set
 *	   CONNECT_WRONG if connect wrong
 *	   OTHER_WRONG other wrong
 *	   f is the fuction you want do with each pair of <domain ,ip>
 *	   nbuf number of domains
 */

static int tcp_dns_search(char *buf, int nbuf, void(*f)(char*, uint32 *))
{
	int i, misscount=0;
	uint32 tempip, total_length, index[nbuf];
	struct iovec iovec[2];
	
	i = pre_do(buf, &total_length, index, nbuf);
	if(SUCCESS != i )
		return i;

        int sockfd;
        struct sockaddr_in servaddr;
        sockfd = socket(AF_INET, SOCK_STREAM, 0); 
        bzero(&servaddr, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_port  = htons(PORT);
        inet_pton(AF_INET, SERVIP, &servaddr.sin_addr);
        if( connect(sockfd, (struct sockaddr *)&servaddr, 
                sizeof(servaddr)) != 0){
                printf("connect failed\n");
		return CONNECT_WRONG;
	}
	
        uint8 *headbuf = (uint8 *)malloc(HEAD_LENGTH*sizeof(uint8));
	build_head(headbuf, total_length);

	iovec[0].iov_base = headbuf;
	iovec[1].iov_base = buf;
	iovec[0].iov_len  = HEAD_LENGTH;
	iovec[1].iov_len  = total_length - HEAD_LENGTH ;


	if(writev(sockfd, iovec, 2) != total_length ){
		printf("writev wrong \n");
		return OTHER_WRONG;
	}
	
	
	uint8 *retbuf = (uint8 *)malloc(nbuf * 2 * sizeof(uint32) +
				 HEAD_LENGTH * sizeof(uint8));
	//first return
	int count = 0;
	while( (i = read(sockfd, retbuf + count, HEAD_LENGTH * sizeof(uint8) +
			nbuf*sizeof(uint32))) > 0){
		count += i;
		if(( HEAD_LENGTH *sizeof(char) + nbuf * sizeof(uint32)) <= count)
			break;
	}
	count = 0;
	if(FIRST_WITHOUT_ERROR == *(uint8 *)(retbuf + HEAD_LENGTH -1)){ //first return without error
		for(i = 0; i < nbuf ; i++){
			f(buf + count , (uint32 *)(retbuf + HEAD_LENGTH) + i);
				//call the f to do with <domain, ip>
			count += strlen(buf + count ) + 1;
		}					
        	close(sockfd);
		return   SUCCESS;
	}
	misscount = 0;
	for(i = 0; i < nbuf ; i++){
		tempip = *(uint32 *)((uint32 *)(retbuf + HEAD_LENGTH )+ i);
		if(tempip == dns_error || tempip ==  cache_miss)
			misscount++;
		else f(buf + index[i] , (uint32 *)(retbuf + HEAD_LENGTH) + i);
			//call the f to do with <domain, ip>
	}		

	//second return;
	count = 0;
	while( (i = read(sockfd, retbuf + count, HEAD_LENGTH * sizeof(uint8)
					+ misscount * 2 * sizeof(uint32))) > 0){
		count += i;
		if((misscount * 2 * sizeof(uint32) +  HEAD_LENGTH) <= count)
			break;
	}
	
	i = 0;
	for(i = 0; i < misscount ; i++){
		f(buf + index[*((uint32 *)(retbuf + HEAD_LENGTH) + 2 * i)], 
		  (uint32 *) ((uint32 *)(retbuf + HEAD_LENGTH) + 2 * i + 1));
			//call the f to do with <domain, ip>
	}

	free(headbuf);
	free(retbuf);
        close(sockfd);
	return   SUCCESS;

}


/*  do dns search with udp connection.
 *  return SUCCESS  success,
 *	   NOT_IN_SET there is character in buf not in the code set
 *	   CONNECT_WRONG if connect wrong
 *	   OTHER_WRONG other wrong
 *	   f is the fuction you want do with each pair of <domain ,ip>
 *	   nbuf number of domains
 */
static int  udp_dns_search(char *buf, int nbuf, void (*f)(char *,uint32 *))
{
	int i, misscount=0;
	uint32 tempip, total_length, index[nbuf];
	struct iovec iovec[2];
        int sockfd;
        struct sockaddr_in servaddr;
	struct msghdr msgsend;
	bzero(&servaddr, sizeof(servaddr));
	bzero(&msgsend,  sizeof(msgsend));
 
 	servaddr.sin_family = AF_INET;
        servaddr.sin_port  = htons(PORT);
        inet_pton(AF_INET, SERVIP, &servaddr.sin_addr);
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);

	i = pre_do(buf, &total_length, index, nbuf);
	if(SUCCESS != i)
		return i;

        uint8 *headbuf = (uint8 *)malloc(HEAD_LENGTH*sizeof(uint8));
	build_head(headbuf, total_length);

	msgsend.msg_name = &servaddr;
	msgsend.msg_namelen = sizeof(servaddr);
	msgsend.msg_iov = iovec;
	msgsend.msg_iovlen = 2;
	iovec[0].iov_base = headbuf;
	iovec[1].iov_base = buf;
	iovec[0].iov_len  = HEAD_LENGTH;
	iovec[1].iov_len  = total_length - HEAD_LENGTH ;
	
	if(sendmsg(sockfd, &msgsend, 0) != total_length){
		perror("sendmsg");
		return OTHER_WRONG;
	}
	
	uint8 *retbuf = (uint8 *)malloc(nbuf * 2 * sizeof(uint32) +
				 HEAD_LENGTH * sizeof(uint8));

	int count = 0;
	count = recvfrom(sockfd, retbuf + count, HEAD_LENGTH * sizeof(uint8) +
		nbuf * sizeof(uint32), 0, NULL, NULL);
	
	for(i = 0; i < nbuf ; i++){
		f(buf + index[i] , (uint32 *)(retbuf + HEAD_LENGTH) + i);
			//call the f to do with <domain, ip>
		count += strlen(buf + count ) + 1;
	}					

	
	free(headbuf);
	free(retbuf);
	return   SUCCESS;
	

}


/*
 * by synckey
 * build  the message head
 * 
 */

static void	build_head(uint8* headbuf, uint32 total_length )
{
	*((uint32 *)headbuf) = total_length;
	*((uint32 *)(headbuf + TOTAL_LENGTH)) = MAGIC_NUMBER;
	*((uint32 *)(headbuf + TOTAL_LENGTH + MAGIC_NUMBER_LENGTH)) = 1;
	*((uint8 *)(headbuf + TOTAL_LENGTH + MAGIC_NUMBER_LENGTH +
		REQUEST_NUMBER_LENGTH))= RESERVED_BYTE ;
	
}
/*
 * by synckey
 * gather the total_length and index  info of the buf
 * return  NOT_IN_SET   if there is character not in the
 * 			code set.
 *	   SUCCESS      if it works well
 *
 */
static int pre_do(uint8* buf, uint32* total_length, uint32* index, int nbuf)
{
	
	inet_pton(AF_INET, DNS_ERROR, &dns_error);
	inet_pton(AF_INET, CACHE_MISS, &cache_miss);
	int i;
	*total_length = 0;
	index[0] = 0;
	for(i = 1; i <= nbuf ; i++){
		while('\0' != buf[*total_length]){
			if(!is_in_set(buf + *total_length))
				return NOT_IN_SET;
			(*total_length)++;
		}
		(*total_length)++;
		if(i != nbuf)
			index[i] =  *total_length ;
	}

	*total_length += HEAD_LENGTH;
	return SUCCESS ;
	
}


/*
 * by wakemecn
 * determine a character is in the dc_set;
 * RETURN 1 if in
 * RETURN 0 if not in
 */
static int is_in_set(char * c)
{	
	
	/* if c pointer to 'a'-'z' or '.' */
	if((*c >= 97 && *c <= 122) || (*c == 46))
		return 1;
	/* if c pointer to '-' */
	if(*c == 45)
		return 1; 
	/* if c pointer to '0'-'9' */
	if(*c >= 48 && *c <= 57 )
		return 1;
	/* if c pointer to 'A'-'Z' */
	if(*c >= 65 && *c <= 90){
		*c = *c + 32;  /* 'a' - 'A' = 32 */
		return 1;
	}
	if(*c == '_')
		return 1;
	printf("%c", *c);
	return 0;
}



/***************  END OF servtest.c  **************/
