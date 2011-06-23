/*
 * Dnscache's memory management
 * Created by gmsh for stub
 * Maintained by wakemecn
 */
#ifndef _DC_MM_H
#define _DC_MM_H

#include <stdlib.h>
void * dc_alloc(size_t size)
{
  return malloc(size);
}

void dc_free(void * ptr)
{
  free(ptr);
}
