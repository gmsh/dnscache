#include "dl_kache.h"
#include <stdio.h>
#include <stdlib.h>
#include "assert.h"

char buff[80];
static void
replace_n ()
{
  int i;
  for (i = 0; i < 80; i++)
    {
      if (buff[i] == '\n')
	{
	  buff[i] = '\0';
	  break;
	}
    }
  buff[79] = '\0';
}

int
main ()
{
  kache_init ();

  FILE *domains = fopen ("./out.list", "r");
  while (NULL != fgets (buff, 80, domains))
    {
      replace_n ();
      data_t *data = kache_retrieve (buff);
      printf ("%u:%s\n", data->ip, buff);
    }
  data_t fake_data;
  fake_data.ip = 12345678;
  fake_data.timestamp = 87654321;
  uint8 *key_ = "asdffdsa.sds";
  kache_store (key_, &fake_data);

  data_t *temp = kache_retrieve (key_);
  assert (temp != NULL);
  printf ("%u, %lu", temp->ip, temp->timestamp);
  putchar (*(char *) (temp + 1));
  putchar (*((char *) (temp + 1) + 1));
  putchar (*((char *) (temp + 1) + 2));
  putchar (*((char *) (temp + 1) + 3));
  putchar ('\n');
  uint8 *key__ = "asd.dsa";
  temp = kache_retrieve (key__);
  assert (temp == NULL);
  fclose (domains);
  kache_exit ();
}
