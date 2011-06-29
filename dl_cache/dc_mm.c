#include "dc_mm.h"
void * dc_alloc(size_t size){
  return malloc(size);
}

void dc_free(void * ptr){
  free(ptr);
}

void * dc_realloc(void * ptr, size_t size)
{
  return realloc(ptr, size);
}
