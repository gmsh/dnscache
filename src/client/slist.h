/*
 * Single linked list using static memory allocation
 * Author gmsh
 * Modified by wakemecn with add function find() and delete();
 */
#ifndef _SLIST_H_
#define _SLIST_H_

#include <stdlib.h>
struct sl_node
{
  void *data;
  struct sl_node *next;
};

struct slist
{
  struct sl_node *head;
  struct sl_node *end;
  struct sl_node *blank;	/* list of blank sl_node */
  size_t length;
  size_t capacity;
  /* list of memory, used by sl_free() */
  struct sl_node *memlist;
};

/*
 * to make a empty slist from given memory
 * allocation function.
 */
struct slist *mk_slist (void *(*my_alloc) (size_t), size_t capacity);

/*
 * to expand the slist delta more free sl_nodes
 *
 */
struct slist *sl_expand (struct slist *sl,
			 void *(my_alloc) (size_t), size_t delta);
/*
 * to add a sl_node who points to *p to the end of list
 * RETURN
 *  0 if success
 *  1 if no memory
 */
int append (void *data, struct slist *sl);

/*
 * to pop a sl_node from the head of list.
 * NEVER try to pop an empty stack/list.
 * RETURN pointer to its data
 */
void *pop (struct slist *sl);

/*
 * to push a sl_node to the head of list
 * RETURN 
 * 0 if success 
 * 1 if no memory
 */
int push (void *data, struct slist *sl);

/*
 * traverse
 * implement void visit(struct sl_node *) then call it
 */
void traverse (void (*visit) (struct sl_node *), struct slist *sl);

/*
 * return pointer to sl_node whose data point to
 * datai.
 * return 0 if not exists and 1 if exists.
 */
int find (void *data, struct slist *sl);

/*
 * Move a node to slist's head
 * RETURN 0 if found the data and mv2head
 * RETURN 1 if not found
 */
int mv2head (void *data, struct slist *sl);

/*
 * free the slist
 */
void sl_free (void (*my_free) (void *), struct slist *sl);
#endif
