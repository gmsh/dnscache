#ifndef DL_KACHE_H
#define DL_KACHE_H

#include "typedefs.h"
#include <stdlib.h>
#define TRIEPATH "./domains.tri"
#define TRIESIZE (1843314 + 1)
#define IPLISTPATH "./ip.list"

typedef struct{
  uint32 ip;
  time_t timestamp;
} data_t;

void kache_init();

void * kache_retrieve_lock(uint8 * key);

void kache_unlock(uint8 *key);

void kache_store(uint8 * key, data_t * data);

void kache_exit();

#endif
