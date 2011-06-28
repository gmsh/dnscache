

/********************************************************************
 * filename:		servtest.c
 * author: 		synckey
 * version:		v1.0
 * datetime:		2011-06-27 21:37:11
 * description:		test the serv_thread_make.c
 ********************************************************************/
#include "server.h"

#define  INPUTFILE "out.list"
#define  LINES      5
int
main()
{
	FILE *fp ;
        int i =0 ;
        fp = fopen(INPUTFILE, "r");
	uint32 total_length = 0;
        char readbuff[4096];
        while(fgets(readbuff, 4096, fp) != NULL){
        	total_length += strlen(readbuff);
		if (LINES == ++i)
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
        inet_pton(AF_INET, "121.250.222.111", &servaddr.sin_addr);
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
	uint32 *retbuf = (uint32 *)malloc(LINES*sizeof(uint32));
 	read(sockfd, buf, LINES*sizeof(uint32));
        char ipstr[16];
        for(i = 0 ; i < 16 ;i++)
                ipstr[i] = '\0';
        for(i = 0; i<LINES; i++){
        inet_ntop(AF_INET, (in_addr_t *)(buf+i), ipstr , 16);
                printf("%s\n", ipstr);
        }   
    

        //read(sockfd, buf, 100);
        close(sockfd);
	fclose(fp);

}
/***************  END OF servtest.c  **************/
