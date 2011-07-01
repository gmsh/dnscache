#include "double_array.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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
  return 0;
}
