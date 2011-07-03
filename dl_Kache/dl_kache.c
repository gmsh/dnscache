#include "dl_kache.h"
#include "trie.h"
#include "dc_mm.h"
#include <assert.h>
#include <string.h>

static Trie *main_tr;
static data_t *main_tr_data;

static inline void char2alphachar(uint8 * chars, AlphaChar * a_chars)
{
  int i;
  for(i = 0; i < strlen(chars) + 1; i++)
    a_chars[i] = chars[i];
}

void kache_init()
{
  main_tr = trie_new_from_file(TRIEPATH);
  assert(main_tr != NULL);
  main_tr_data = (data_t *)dc_alloc(sizeof(data_t) * TRIESIZE);
  assert(main_tr_data != NULL);
  time_t current_time = time(NULL);
  int i;
  char * s;
  long temp;
  FILE * iplist = fopen(IPLISTPATH, "r");
  uint8 * buff = (uint8 *)dc_alloc(80);
  assert(iplist != NULL);
  for(i = 1; i <= TRIESIZE; i++){
    char *s = fgets(buff, 80, iplist);
    assert(s != NULL);
    temp = atol(buff);
    main_tr_data[i].ip = (uint32) temp;
    main_tr_data[i].timestamp = current_time;
  }
  dc_free(buff);
  fclose(iplist);
  printf("dl_Kache init OK");
}

void * kache_retrieve_lock(uint8 * key)
{
  if(NULL == key)
    return NULL;
  AlphaChar * temp = dc_alloc((strlen(key) + 1) * sizeof(AlphaChar));
  char2alphachar(key, temp);
  uint32 index;
  Bool a = trie_retrieve(main_tr, temp, &index);
  dc_free(temp);
  if(!a)
    return NULL;
  return main_tr_data + index;
}

void kache_unlock(uint8 *key)
{
  /* do nothing here. */
}

void kache_store(uint8 * key, data_t * data)
{
  if(NULL == key)
    return;
  AlphaChar * temp = dc_alloc((strlen(key) + 1) * sizeof(AlphaChar));
  char2alphachar(key, temp);
  uint32 index;
  Bool a = trie_retrieve(main_tr, temp, &index);
  dc_free(temp);
  if(a){/* in main trie */
    memcpy(main_tr_data + index, data, sizeof(data_t));
  }else{
    /* do nothing here. */
  }
}

void kache_exit()
{
  dc_free(main_tr);
  dc_free(main_tr_data);
}
