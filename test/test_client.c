

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
#define LINES  1600

#define INPUTFILE	"domain.list"

void f(char* , uint32 *);

int main(int argc, char **argv)
{	
	FILE *fp;
	if(NULL == (fp = fopen(INPUTFILE, "r"))){
		perror("fopen");
		exit(0);
	}
		
	char *buf;
	buf=malloc(LINES * 64 *sizeof(char));
	char readbuf[4096];
	uint32 readcount = 0;
	int i = 0;
	 while(fgets(readbuf , 4096, fp) != NULL){
	 	readbuf[strlen(readbuf) - 1 ] = '\0';
		strcpy(buf + readcount, readbuf);
		readcount += strlen(readbuf) + 1;
		if(++i >= LINES)
			break;
	} 
	dns_search(buf, LINES, f);
	printf("done\n");

}

void f(char *domain , uint32 *ip)
{	
	char ipstr[16];
	printf("%s\t", domain);
	inet_ntop(AF_INET, (in_addr_t *)ip, ipstr , 16);
	printf("%s\n", ipstr);
}
/***************  END OF test_client.c  **************/
