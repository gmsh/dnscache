/*
 * Dnscache's memory management
 * Created by gmsh for stub
 * Maintained by wakemecn
 */
#ifndef _DC_MM_H
#define _DC_MM_H

#include <stdlib.h>
/*
 * the main thread calls mm_init() to initialize the momery managment 
 * modules.
 * RETURN 0 if succeed;
 * RETURN 1 if failed.
 */
int mm_init();

/* dc_alloc() allocates size bytes and returns a pointer  to  the  allocated
 * memory.   The  memory  is  not  cleared.   If  size is 0, then dc_alloc()
 * returns either NULL, or a unique pointer value that can later  be  suc‐
 * cessfully passed to dc_free().
 */
void * dc_alloc(size_t size)
{
  return malloc(size);
}

/*
 * dc_free()  frees  the memory space pointed to by ptr, which must have been
 * returned by a previous call to dc_alloc().  Otherwise,  or if free(ptr)
 * has already been called before, undefined behavior
 * occurs.  If ptr is NULL, no operation is performed.
 */
void dc_free(void * ptr)
{
  free(ptr);
}
#endif
