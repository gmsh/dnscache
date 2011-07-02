/*
 * filename: test_client.c
 * author:   wakemecn
 * date:     Jul 2nd, 2011
 * description: imitation of client.
 */

//#include "dc_client.h"
#include <stdio.h>
#include <arpa/inet.h>

#define INPUT_FILE "input.list"
#define OUTPUT_FILE "output.list"
#define DN_NUM_LIMIT 5000
#define uint32	int
#define DN_LENGTH 63
#define DN_BUF_LENGTH (DN_NUM_LIMIT * (DN_LENGTH + 1))

/*
 * write the (domain name, ip) into file.
 * RETURN 
 */
static int write_into_file(char *, uint32 *);

/*
 * print to screen the (domain name, ip)
 * RETURN
 */
static int print(char *, uint32 *);

/* create domain names buf 
 * return num of domain name
 * return 0 if no domain names 
 */
static uint32 create_buff(char *);

int main()
{
	char * dn_buff;
	uint32 dn_num;
    if(dn_num = create_buff( dn_buff))
		dns_search(dn_buff, dn_num, write_into_file);
	return 0;	
}

static uint32 create_buff(char * dn_buf)
{
	FILE * infp;  /* input file pointer. */
	uint32 dn_num = 0;
	char tem_buf[DN_LENGTH + 2];
	/* TODO modify dinamic mem */
	dn_buf = char[DN_BUF_LENGTH];
	char * pos = dn_buf;
	if((infp = fopen(INPUT_FILE, 'r')) == NULL){
		printf("client : %s\n", " domain list file read error");
		exit(1);
	}
	while((fgets(tem_buf, DN_LENGTH + 2), infp) == NULL){
		tem_buf[strlen(tem_buf) - 1] = '\0';
		strcpy(pos, tem_buf);
		pos = pos + strlen(tem_buf) + 1;
		dn_num++;
		if(dn_num >= DN_NUM_LIMIT)
			break;
	}
	if(fclose(infp) == EOF){
		printf("client : %s\n", " domain list file close error");
		exit(1);
	}
	return dn_num;
}

static int print(char *, uint32 *)
{
	char ipstr[16];
	printf("%s\t", domain);
	inet_ntop(AF_INET, (in_addr_t *)ip, ipstr , 16);
	printf("%s\n", ipstr);
}

static int write_into_file(char * dn, uint32 * ip)
{
	FILE * outfp;
	char ipstr[16], line[strlen(dn) + 20]; /* 20 = '\n' + 16 + '\n'*/
	char * ptr;
	strcpy(line, dn);
	line[strlen(line) + 1] = '\n';
	line[strlen(line) + 2] = ' ';
	line[strlen(line) + 3] = ' ';
	inet_ntop(AF_INET, (in_addr_t *)ip, ipstr , 16);
	strcpy(&line[strlen(line) + 4], ipstr);
	line[strlen(dn) + 19] = '\n';
	if((outfp = fopne(OUTPUT_FILE, 'a')) == NULL){
		printf("client : %s\n", " domain, ip list file open error");
		exit(1);
	}
	fputs(line, outfp); 
	if(fclose(infp) == EOF){
		printf("client : %s\n", " domain, ip list file close error");
		exit(1);
	}
}
