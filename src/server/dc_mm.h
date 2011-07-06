/*
 * Dnscache's memory management
 * Created by gmsh for stub
 * Maintained by wakemecn
 */
#ifndef _DC_MM_H
#define _DC_MM_H

#include <stdlib.h>
#include <stdio.h>
#include "slist.h"

/*
 * the main thread calls mm_init() to initialize the momery managment 
 * modules.
 */
void mm_init(void);

/* dc_alloc() allocates size bytes and returns a pointer  to  the  allocated
 * memory.   The  memory  is  not  cleared.   If  size is 0, then dc_alloc()
 * returns either NULL, or a unique pointer value that can later  be  suc‐
 * cessfully passed to dc_free().
 */
void * dc_alloc(size_t size);

/*
 * dc_free()  frees  the memory space pointed to by ptr, which must have been
 * returned by a previous call to dc_alloc().  Otherwise,  or if free(ptr)
 * has already been called before, undefined behavior
 * occurs.  If ptr is NULL, no operation is performed.
 */
void dc_free(void * ptr);

/*
 * The  dc_realloc() function changes the size of the memory block pointed to
 * by ptr to size bytes.  The contents will be unchanged in the range from
 * the start of the region up to the minimum of the old and new sizes.  If
 * the new size is larger than the old size, the added memory will not  be
 * initialized.   If  ptr  is  NULL,  then  the call is equivalent to dc_al‐
 * loc(size), for all values of size; if size is equal to zero, and ptr is
 * not  NULL,  then  the  call  is equivalent to dc_free(ptr).  Unless ptr is
 * NULL, it must have been returned by an earlier call to  dc_alloc(),
 * dc_realloc().  If the area pointed to was moved, a dc_free(ptr) is done.
 */
void * dc_realloc(void * ptr, size_t size);

/* print the statics about the apply and free of chunks.*/
void print_statics();

#endif
