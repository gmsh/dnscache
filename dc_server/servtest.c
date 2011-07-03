

/********************************************************************
 * filename:		servtest.c
 * author: 		synckey
 * version:		v1.0
 * datetime:		2011-06-27 21:37:11
 * description:		test the serv_thread_make.c
 ********************************************************************/
#include "server.h"

#define  INPUTFILE "domain.list"
#define  OUTPUTFILE "domainip.list"
#define  IPFILE "ip.list"
int
main()
{
	FILE *fp, *outfp, *ipfp;
        int i =0 ;
       	if( (fp = fopen(INPUTFILE, "r")) == NULL)	
		printf("file open error\n");
 
 	if( (outfp = fopen(OUTPUTFILE, "w+")) == NULL){
		printf("file open error\n");
		exit(0);
	}
 	if( (ipfp = fopen(IPFILE, "w+")) == NULL){
		printf("file open error\n");
		exit(0);
	}
	
     //   fp = fopen(OUTPUTFILE, "rw");
	uint32 total_length = 0;
        char readbuff[4096];
        while( fgets(readbuff, 4096, fp) != NULL){
        	total_length += strlen(readbuff);
		i++;
		if (LINES == i)
			break;
        } 
	total_length += HEAD_LENGTH;
	fseek(fp, 0, SEEK_SET);



        int sockfd;
        struct sockaddr_in servaddr;
        sockfd = socket(AF_INET, SOCK_STREAM, 0); 
        bzero(&servaddr, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_port  = htons(PORT);
        inet_pton(AF_INET, SERVIP, &servaddr.sin_addr);
        if( connect(sockfd, (struct sockaddr *)&servaddr, 
                sizeof(servaddr)) != 0)
                printf("connect failed\n");
        else    printf("conneted \n");
	
        uint8 *buf = (uint8 *)malloc(total_length*sizeof(uint8));


	*((uint32 *)buf) = total_length;
	*((uint32 *)(buf + TOTAL_LENGTH)) = MAGIC_NUMBER;
	*((uint32 *)(buf + TOTAL_LENGTH + MAGIC_NUMBER_LENGTH)) = 1;
	*((uint8 *)(buf + TOTAL_LENGTH + MAGIC_NUMBER_LENGTH +
		REQUEST_NUMBER_LENGTH))= 'F' ;
        i = 0;
	uint32 readcount = HEAD_LENGTH;
	while(fgets(readbuff, 4096, fp) != NULL){
		readbuff[strlen(readbuff) - 1] = '\0';
		strcpy(buf + readcount, readbuff);
       		readcount += strlen(readbuff) + 1;
		if (++i == LINES)
			break;
        } 
	
	write(sockfd, buf, total_length);
	uint8 *retbuf = (uint8 *)malloc(LINES * 2 * sizeof(uint32) +
			HEAD_LENGTH * sizeof(uint8));
 	
	//first return
	int retcount = 0;
	while( (i = read(sockfd, retbuf + retcount,
		HEAD_LENGTH * sizeof(uint8) + LINES*sizeof(uint32))) > 0){
		retcount += i;
		printf("i %ld", i);
		printf("retcount %d\n", retcount);
		if(( HEAD_LENGTH + LINES*sizeof(uint32)) <= retcount)
			break;
	}
	printf("recieved \n");
        char ipstr[16]; 
        for(i = 0 ; i < 16 ;i++)
                ipstr[i] = '\0';
	int len = HEAD_LENGTH ; 
        for(i = 0; i < LINES; i++){
		printf("%s   ", buf + len);
		len += strlen (buf + len) + 1;
 		inet_ntop(AF_INET, (in_addr_t *)( (uint32 *)(retbuf +HEAD_LENGTH) + i), ipstr , 16);
                printf("%s\n", ipstr);
        } 

    	//second return
	retcount = 0;
	while( (i = read(sockfd, retbuf + retcount,HEAD_LENGTH * sizeof(uint8)
					+ LINES * 2 * sizeof(uint32))) > 0){
		retcount += i;
		if((LINES * 2 * sizeof(uint32) +  HEAD_LENGTH) <= retcount)
			break;
	}
	printf("recieved \n");
        for(i = 0 ; i < 16 ;i++)
                ipstr[i] = '\0';
	len = HEAD_LENGTH ; 
        for(i = 0; i < LINES; i++){
		fprintf(outfp,"%s\t", buf + len);
		printf("index %d   ", *(uint32 *)((uint32 *) (retbuf + HEAD_LENGTH ) + i * 2   ));
		printf("%s\t", buf + len);
		len += strlen (buf + len) + 1;
 		inet_ntop(AF_INET, (in_addr_t *)((uint32 *)(retbuf + HEAD_LENGTH) + i * 2 + 1), ipstr , 16);
                printf("%s\n", ipstr);
                fprintf(outfp, "%s\n", ipstr);
             	fprintf(ipfp, "%u\n",  *((in_addr_t *)((uint32 *)(retbuf +
		HEAD_LENGTH) + i * 2 + 1)));
	}

	printf("done\n");
        close(sockfd);
	fclose(fp);
	fclose(ipfp);
	fclose(outfp);

}
/***************  END OF servtest.c  **************/
