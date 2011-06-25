

/**********************************************************
 * filename:		dnsrequest.c
 * author: 		synckey
 * version:		v1.0
 * datetime:		2011-06-25 20:34:57
 * description:		connect to the dns server and 
 * 			return the ip addres of the requied
 *			domain.
 **********************************************************/
#include "server.h"

/*
 * input: name point to the domain name you want to translate
 *	to ip,ipaddr point to the address you want to put ip.
 * 
 * return: 0 success, else faild.
 * 
 */
int dnsrequest(const char *name ,uint32 *ipaddr)
{
	struct addrinfo		hints, *res ;
	int	retval;
	bzero(&hints, sizeof(hints));
	hints.ai_family = AF_INET;	//IPv4 address
	
	retval = getaddrinfo(name, "domain", &hints, &res);

	if(retval != 0){
		return retval;
	}
	
	if(res != NULL){
		*ipaddr =(in_addr_t) (((struct sockaddr_in *)(res->ai_addr))
				->sin_addr).s_addr;
	}
		
	freeaddrinfo(res);
	return retval;
}

/***************  END OF dnsrequest.c  **************/
