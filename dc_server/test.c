

/********************************************************************
 * filename:		test.c
 * author: 		synckey
 * version:		v1.0
 * datetime:		2011-06-25 22:17:25
 * description:		test the functions 
 ********************************************************************/

#include "server.h"


int dnsrequest(char *, uint32 *);

int main(int argc, char **argv)
{	

	


	/*test the function dnsrequest */
	/*
	uint32 addr;
	char ipstr[16];
	int i;
	for(i = 0 ; i < 16 ;i++)
		ipstr[i] = '\0';
	int a = dnsrequest("www.931momo.cn", &addr);
	inet_ntop(AF_INET, (in_addr_t *)&addr, ipstr, 16);
	printf("%s\n", ipstr);
	if( a != 0)
		printf("%s\n", gai_strerror(a));
	return(0);
	*/
}

/***************  END OF test.c  **************/
