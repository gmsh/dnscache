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
  Trie *tr = trie_new_from_file ("domains.tri");
  TrieData var;
  FILE *domains = fopen ("./out.list", "r");
  while (NULL != fgets (buff, 80, domains))
    {
      replace_n ();
      char2alphachar ();
      // printf("%s\n", buff);
      Bool a = trie_retrieve (tr, key, &var);
      if (a)
	printf ("%d\n", var);
    }
  trie_free (tr);
}
