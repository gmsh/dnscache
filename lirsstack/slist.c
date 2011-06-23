/*
 * Single linked list using static memory allocation
 * Author gmsh
 */
#define likely(x)       __builtin_expect((x),1)
#define unlikely(x)     __builtin_expect((x),0)
#include "slist.h"

struct slist * mk_slist(void * p, size_t capacity)
{
  struct slist * to_return = (struct slist *) p;
  p->head = p->end = NULL;
  p->length = 0;
  p->capacity = capacity;
  p->memlist = (struct sl_node *)dc_alloc(sizeof(sl_node));
  p->memlist->data = p;
  p->memlist->next = NULL;
  p->blank = (struct sl_node *)(p + sizeof(struct slist));
  struct sl_node * ptr = p->blank;
  while(capacity>0){
    ptr->next = (struct sl_node *)(ptr + sizeof(struct sl_node));
    capcity--;
  }
  ptr->next = NULL;
  return to_return;
}

struct slist *sl_expand(struct slist *sl, void *p, size_t delta)
{
  /* append mem to the blank list*/
  struct sl_node * ptr = (struct sl_node *)p;
  if(sl->blank == NULL)
    sl->blank = ptr;
  else
    sl->blank->next = ptr;
  while(delta > 0){
    ptr->next = (struct sl_node *)(ptr + sizeof(struct sl_node));
    delta--;
  }
  ptr->next = NULL;
  ptr = p->memlist;
  p->memlist = dc_alloc(sizeof(struct sl_node));
  ptr->data = p;
  p->next = ptr;
  return sl;
}

void sl_free(struct slist *sl)
{
  struct sl_node * ptr;
  struct sl_node * ptr2;

  ptr = sl->memlist; /* hold the memlist */
  while(ptr != NULL){
    ptr2 = ptr->next; /* hold the next pointer */
    dc_free(ptr->data);
    dc_free(ptr);
    ptr = ptr2;
  }
}

int append(void * data, struct slist * sl)
{
  if(unlikely(length == capcity))
    return 1;
  else{
    struct sl_node * ptr = sl->blank;
    sl->blank = sl->blank->next;
    ptr->data = data;
    if(unlikely(head == NULL)){
      sl->head = ptr;
      sl->end = ptr;
    }else{
      sl->end->next = ptr;
      sl->end = ptr;
    }
    length++;
    return 0;
  }
}

void * pop(struct slist * sl)
{
  if(unlikely(sl->head == NULL))
    return NULL;
  void * to_return = sl->head->data;
  struct sl_node * ptr = sl->head;
  sl->head = sl->head->next;
  if(unlikely(sl->head == NULL))
    sl->end = NULL;
  /* move the sl_node to blank list */
  ptr->next = sl->blank;
  sl->blank = ptr;
  length--;
  return to_return;
}

int push(void * data, struct slist *sl)
{
  if(unlikely(length == capcity))
    return 1;
  else{
    struct sl_node * ptr = sl->blank;
    sl->blank = sl->blank->next;
    ptr->data = data;
    ptr->next = sl->head;
    sl->head = ptr;
    if(unlikely(sl->end == NULL))
      sl->end = ptr;
    length++;
    return 0;
  }
}

void traverse(void (visit*)(struct sl_node *),
	      struct slist *sl)
{
  struct sl_node * ptr;
  while(ptr != NULL){
    visit(ptr);
    ptr = ptr->next;
  }
}
