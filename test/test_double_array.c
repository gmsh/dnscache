#include "double_array.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main()
{
  double_array * da = new_double_array();
  char * key = (char *)malloc(10);
  int i;
  strcpy(key, "abc.com");
  da_insert(key, &i, da);
  while(1);
  return 0;
}
