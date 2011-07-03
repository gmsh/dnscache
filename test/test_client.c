/*
 * filename: test_client.c
 * author:   wakemecn
 * date:     Jul 2nd, 2011
 * description: imitation of client.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "dc_client.h"

#define INPUT_FILE "input.list"
#define OUTPUT_FILE "output.list"
#define DN_NUM_LIMIT 50
#define uint32	int
#define DN_LENGTH 63
#define DN_BUF_LENGTH (DN_NUM_LIMIT * (DN_LENGTH + 1))
#define THREAD_NUM 300 

/*
 * write the (domain name, ip) into file.
 * RETURN 0 if success
 * RETURN 1 if failed
 */
static int write_into_file(char *, uint32 *);

/*
 * print to screen the (domain name, ip)
 */
static void print(char *, uint32 *);

/* create domain names buf 
 * return num of domain name
 * return 0 if no domain names 
 */
static uint32 create_buf(char *);

/*
 * encapsulation of fun dns_search()
 */
void * ecp_dns_search(void * );

int main()
{
	/* mutil thread */
	int num = THREAD_NUM;
	pthread_t pt[THREAD_NUM];
	void * arg[THREAD_NUM][3];
	while(num--){
		arg[num][0] = malloc(DN_BUF_LENGTH);
		arg[num][1] = malloc(sizeof(int));
		*(int *)arg[num][1] = create_buf((char *)arg[num][0]);
		arg[num][2] = print;
		//arg[num][2] = write_into_file;
	}
	num = THREAD_NUM;
	while (num--){
		pthread_create(&pt[num], NULL, ecp_dns_search, arg[num]);
		pthread_join(pt[num], NULL);
	}
	/* single thred */
/*	
	char * dn_buf;
	uint32 dn_num;
	dn_buf = (char *)malloc(DN_BUF_LENGTH);
    if(dn_num = create_buf(dn_buf))
//		dns_search(dn_buf, dn_num, write_into_file);
		dns_search(dn_buf, dn_num, print);
*/
//	sleep(20);
	return 0;	
}

void * ecp_dns_search(void * args)
{
	dns_search((char *)((void **)args)[0], *(int *)((void **)args)[1], 
	           ((void **)args)[2]);
}

static uint32 create_buf(char * dn_buf)
{
	FILE * infp;  /* input file pointer. */
	uint32 dn_num = 0;
	char tem_buf[DN_LENGTH + 2];
	/* TODO modify dinamic mem */
	//char * pos = dn_buf;
	if((infp = fopen(INPUT_FILE, "r")) == NULL){
		printf("client : %s\n", " domain list file read error");
		return 0;
	}
	while(fgets(tem_buf, DN_LENGTH + 2, infp) != NULL){
		tem_buf[strlen(tem_buf) - 1] = '\0';
		strcpy(dn_buf, tem_buf);
		dn_buf = dn_buf + strlen(tem_buf) + 1;
		dn_num++;
		if(dn_num >= DN_NUM_LIMIT)
			break;
	}
	if(fclose(infp) == EOF){
		printf("client : %s\n", " domain list file close error");
		return 0;
	}
	return dn_num;
}

static void print(char * domain, uint32 * ip)
{
	char ipstr[16];
	printf("%s\t", domain);
	inet_ntop(AF_INET, (in_addr_t *)ip, ipstr , 16);
	printf("%s\n", ipstr);
}

static int write_into_file(char * dn, uint32 * ip)
{
	FILE * outfp;
	char ipstr[16];
	char * ptr;
	if((outfp = fopen(OUTPUT_FILE, "a")) == NULL){
		printf("client : %s\n", " domain, ip list file open error");
		return 1;
	}
	inet_ntop(AF_INET, (in_addr_t *)ip, ipstr , 16);
	fputs(dn, outfp); 
	fputs("\t", outfp);
	fputs((char *)ipstr, outfp);
	fputs("\n", outfp);
	if(fclose(outfp) == EOF){
		printf("client : %s\n", " domain, ip list file close error");
		return 1;
	}
	return 0;
}
