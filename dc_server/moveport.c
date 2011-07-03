

/********************************************************************
 * filename:		moveport.c
 * author: 		synckey
 * version:		v1.0
 * datetime:		2011-06-27 13:59:22
 * description:		remove the port and ip addr in uniq_domain
			out file will be out.list
 ********************************************************************/
#define  OUTFILE "out.list"
#include<stdio.h>
int main(int argc, char **argv)
{
	FILE *fp , *fpout;
	int i =0 ;
	fp = fopen(argv[1], "r");
	fpout = fopen(OUTFILE, "w+");
	char buff[4096];
	while(fgets(buff, 4096, fp) != NULL){
		i = 0;
		while( (buff[i] !='\n') && (buff[i]) != ':')
			i++;
		buff[i] = '\n';

		if((buff[i-1] <= '9') && (buff[i-1] >= '0')){
			printf("move out a ipaddr\n");
			continue;
		}
		fwrite(buff, sizeof(char), i+1, fpout);
		
	}
	
	fclose(fp);
	fclose(fpout);

}

/***************  END OF moveport.c  **************/
