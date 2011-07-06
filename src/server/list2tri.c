#include "trie.h"
#include "stdlib.h"
#include "stdio.h"

char buff[80];
AlphaChar key[80];
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

static void
char2alphachar ()
{
  int i;
  for (i = 0; i < 80; i++)
    key[i] = buff[i];
}


int
main ()
{
  AlphaMap *am_ = alpha_map_new ();
  alpha_map_add_range (am_, 0x002d, 0x002e);
  alpha_map_add_range (am_, 0x0030, 0x0039);
  alpha_map_add_range (am_, 0x0061, 0x007a);
  alpha_map_add_range (am_, 0x005f, 0x005f);
  Trie *tr_ = trie_new (am_);
  alpha_map_free (am_);

  int var = 1;
  FILE *domains = fopen ("./out.list", "r");
  while (NULL != fgets (buff, 80, domains))
    {
      replace_n ();
      char2alphachar ();
      // printf("%s\n", buff);
      trie_store (tr_, key, var++);
    }
  trie_save (tr_, "./domains.tri");
}
