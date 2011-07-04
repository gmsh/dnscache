#include "dl_kache.h"
#include "trie.h"
#include "dc_mm.h"
#include <assert.h>
#include <string.h>
#include "hash.h"
#include "typedefs.h"

typedef struct{
  data_t h_data;
  uint8 part_key[4];
} hash_data_t;

static Trie *main_tr;
static data_t *main_tr_data;
static hash_data_t *hash_data;

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
  hash_data = dc_alloc(sizeof(hash_data_t) * HASHSIZE);
  assert(hash_data != NULL);
  memset(hash_data, 0, (sizeof(hash_data_t) * HASHSIZE));
  time_t current_time = time(NULL);
  int i;
  char * s;
  long temp;
  FILE * iplist = fopen(IPLISTPATH, "r");
  uint8 * buff = (uint8 *)dc_alloc(80);
  assert(iplist != NULL);
  for(i = 1; i <= TRIESIZE - 1; i++){
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

data_t * kache_retrieve_lock(uint8 * key)
{
  if(NULL == key)
    return NULL;
  AlphaChar * temp = dc_alloc((strlen(key) + 1) * sizeof(AlphaChar));
  char2alphachar(key, temp);
  uint32 index;
  Bool a = trie_retrieve(main_tr, temp, &index);
  dc_free(temp);
  if(a)
    return main_tr_data + index;
  int8 len = strlen(key) > 4 ? 4: strlen(key);
  uint32 hash_val = BKDRHash(key);
  if(0 != strncmp(hash_data[hash_val].part_key, key, len)){
    return NULL;
  }
  return &(hash_data[hash_val].h_data);
}

inline void kache_unlock(uint8 *key)
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
    int8 len = strlen(key) > 4? 4: strlen(key);
    uint32 hash_val = BKDRHash(key);
    memcpy(&hash_data[hash_val].h_data, data, sizeof(data_t));
    memcpy(hash_data[hash_val].part_key, key, len);
  }
}

void kache_exit()
{
  free(main_tr);
  dc_free(main_tr_data);
}
