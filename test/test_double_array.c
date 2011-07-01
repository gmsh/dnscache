#include "double_array.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char buff[80];

static void replace_n(){
	int i;
	for(i = 0; i < 80; i ++){
		if(buff[i]=='\n'){
			buff[i] = '\0';
			break;
		}
	}
	buff[79] = '\0';
}
int main()
{
  double_array * da = new_double_array();
  char * key = (char *)malloc(10);
  int a;
  strcpy(key, "abc.com");
  da_insert(key, &a, da);
  int * b = da_get_data(key, da);
  /* the above check &a == b */
  int c;
  char * key2 = (char *)malloc(10);
  strcpy(key2, "abe.com");
  int * d = da_get_data(key2, da);
  da_insert(key2, &c, da);
  int * e = da_get_data(key, da);
  int * f = da_get_data(key2, da);
  /* 
   * the above check NULL == d
   * e == b
   * &c == f
   */
  free_da(da);
  
  da = new_double_array();
  
  void * var = 0;
  FILE * domains = fopen("./out.list", "r");
  while(NULL != fgets(buff, 80, domains)){
    replace_n();
    printf("%s\n", buff);
    da_insert(buff, (var ++), da);
  }
  fseek(domains, 0, SEEK_SET);
  while(NULL != fgets(buff, 80, domains)){
    replace_n();
    int64 num =(int64)da_get_data(buff, da);
    printf("%ld: %s\n", num, buff);
  }
  return 0;
}
