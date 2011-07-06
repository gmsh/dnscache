/*
 * filename: test_client.c
 * author:   wakemecn
 * date:     Jul 2nd, 2011
 * description: imitation of client, we can configure 
 *				the macro definitions to enhace the 
 *				virtual client, we also could run a l-
 *				ot of copy on different machion to im-
 *				itate the concurrent request.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <unistd.h>
#include <pthread.h>
#include "dc_client.h"
#include "datrie_tail_pool.h"
#include "dc_mm.h"

/*
 * file to initilzation the datrie tree.
 */
#define INPUT_FILE	"domain.list"

/*
 * output file to store the printf message 
 * created by the requested threads.
 */
#define OUTPUT_FILE	"output.list"

/* 
 * the max number of doman names can be requested.
 */
#define DN_NUM_LIMIT 400000

/* 
 * the max length of domain name.
 */
#define DN_LENGTH 63

/*
 * the number of request threads running.
 */
#define THREAD_NUM  12

/*
 * the number of domain names in the datrie tree.
 */
#define DN_NUM 	1843314

/*
 * write the (char * domain name, uint32 * ip)
 * into file,
 * RETURN 0 if success,
 * RETURN 1 if failed.
 */
static int write_into_file(char *, uint32 *);

/*
 * print the (char * domain name, uint32 * ip)
 * to stdout.
 */
static void print(char *, uint32 *);

/* create domain names buf, 
 * the first argument is the pointer to buf,
 * the second argument is the number of domain names,
 * return num of domain name,
 * return 0 if no domain names. 
 */
static void create_buf(char *, uint32);

/*
 * encapsulation of fun dns_search() to adapt the th-
 * read demands of it's working function.
 */
void * ecp_dns_search(void * );

/*
 * To intialize the datrie tail pool,
 * return 0 if success,
 * return 1 if failed.
 */
int init_dtp();

/*
 * replace the '\n' with '\0' in a string.
 */
static void replace_n();

/*
 * pointer to key numbers used for generating random 
 * test set.
 */
int32 * key;
	
/* pointer to datrie tail pool. */
datrie_tail_pool * dtp;

/* a buffer used for store domain names. */
char buff[80];

int main()
{
	/* intialize the datrie tail pool. */
	if(init_dtp()){
		printf("client : %s\n","initialize the datrie tail pool failed.");
		return 1;
	}

	/*
	 * single thread or mutil threads depends on the
	 * user. 
	 */

	/* mutil threads */
	uint32 num = THREAD_NUM, dn_num;
	void * arg[THREAD_NUM][3];
	
	/* one thread imitate a client. */
	pthread_t pt[THREAD_NUM];
	/* generate the fun args for each thread.*/
	while (num--){
		/* generate a random number of domain names */
//		while ((dn_num = rand() % DN_NUM_LIMIT) == 0)
//			;
		dn_num = DN_NUM_LIMIT;
		arg[num][0] = malloc(DN_LENGTH * dn_num);
		arg[num][1] = malloc(sizeof(int));
		*(int *)arg[num][1] = dn_num;
		create_buf((char *)arg[num][0], dn_num);
	
		/* 
		 *choose one of policies below for output, strong 
		 * recommended the first one.
		 */
		arg[num][2] = print;
		//arg[num][2] = write_into_file;
	}
	num = THREAD_NUM;
	/* used to compute the time.*/
	struct timeval * st, * ft;
	st = malloc(sizeof(struct timeval));
	ft = malloc(sizeof(struct timeval));
	gettimeofday(st, NULL);
	
	while (num--)
		pthread_create(&pt[num], NULL, ecp_dns_search, arg[num]);
	num = THREAD_NUM;
	while(num--)
		pthread_join(pt[num], NULL);
	gettimeofday(ft, NULL);
	printf("time used : %f s\n", ((ft->tv_sec - st->tv_sec) * 1000000 + 
	       (ft->tv_usec - st->tv_usec)) / 1000000.0);
	

	/* single thread */
/*	char * dn_buf;
	uint32 dn_num;
    	while((dn_num = rand() % DN_NUM_LIMIT) ==0 )
		;
	dn_buf = (char *)malloc(dn_num * DN_LENGTH);
	create_buf(dn_buf, dn_num);
		//dns_search(dn_buf, dn_num, print);
	dns_search(dn_buf, dn_num, write_into_file);
*/
	return 0;	
}

int init_dtp()
{
	key = malloc(sizeof(int32) * DN_NUM);
	dtp = new_datrie_tail_pool();
	FILE * domains = fopen(INPUT_FILE, "r");
	uint32 i = 0;

	/* create the datrie tree which used for generate 
	 * random test set.
	 */
	while (NULL != fgets(buff, 80, domains)){
		replace_n();
		key[i++] = dt_push_tail(buff, dtp);
	     // printf("key[%d] = %d\n", i, j);
		//key[i] = j;
		//i++;
	}
	fclose(domains);
	return 0;
}

void * ecp_dns_search(void * args)
{	
//	pthread_detach(pthread_self());
	dns_search((char *)((void **)args)[0], *(int *)((void **)args)[1], 
	           ((void **)args)[2]);
}

static void create_buf(char * dn_buf, uint32 dn_num)
{
	while (dn_num--){
		/* get a domain name from datrie tail pool. */
		int x = rand() % DN_NUM;
		uint8 *	tem_buf = dt_get_tail(key[x], dtp);
		strcpy(dn_buf, tem_buf);
		dn_buf = dn_buf + strlen(tem_buf) + 1;
	}

	/* 
	 * also we can create buffer from a file, a domain
	 * name per line, and change the prototype of the
	 * function to static uint32 create_buf(char *) w-
	 * hich return the number of domain names, below 
	 * is the code.
	 */
/*	
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
*/
}

static void replace_n(){
	int i;
	for (i = 0; i < 80; i ++){
		if(buff[i]=='\n'){
			buff[i] = '\0';
			break;
		}
	}
	buff[79] = '\0';
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

	/* open file. */
	if((outfp = fopen(OUTPUT_FILE, "a")) == NULL){
		printf("client : %s\n", " domain, ip list file open"
		       " error");
		return 1;
	}
	/* 32-bit binary to dotted decimal. */
	inet_ntop(AF_INET, (in_addr_t *)ip, ipstr , 16);
	/* write into files. */
	fputs(dn, outfp); 
	fputs("\t", outfp);
	fputs((char *)ipstr, outfp);
	fputs("\n", outfp);
	/* close file.*/
	if(fclose(outfp) == EOF){
		printf("client : %s\n", " domain, ip list file close error");
		return 1;
	}
	return 0;
}
