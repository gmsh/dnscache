

/********************************************************************
 * filename:		test_client.c
 * author: 		synckey
 * version:		v1.0
 * datetime:		2011-07-01 20:19:55
 * description:		test the dc_client.c
 ********************************************************************/

#include "dc_client.h"
#include <stdio.h>
#include <arpa/inet.h>
void f(char* , uint32 *);

int main(int argc, char **argv)
{
	char *a;
	a=malloc(1024*sizeof(char));
	strcpy(a, "www.baidu.com");
	strcpy(a+strlen(a) + 1, "www.google.com");
	dns_search(a, 2, f);

}

void f(char *domain , uint32 *ip)
{	
	char ipstr[16];
	printf("%s\t", domain);
	inet_ntop(AF_INET, (in_addr_t *)ip, ipstr , 16);
	printf("%s\n", ipstr);
}
/***************  END OF test_client.c  **************/
