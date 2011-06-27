/*
 * Single linked list using static memory allocation
 * Author gmsh
 */
#define likely(x)       __builtin_expect((x),1)
#define unlikely(x)     __builtin_expect((x),0)
#include "slist.h"

struct slist * mk_slist(void * ( *my_alloc) (size_t),
			size_t capacity)
{
  void *p = my_alloc(sizeof(struct slist)
		     + sizeof(struct sl_node) * capacity);
  struct slist * to_return = (struct slist *) p;
  to_return->head = to_return->end = NULL;
  to_return->length = 0;
  to_return->capacity = capacity;
  to_return->memlist = (struct sl_node *)my_alloc(sizeof(struct sl_node));
  to_return->memlist->data = p;
  to_return->memlist->next = NULL;
  to_return->blank = (struct sl_node *)(p + sizeof(struct slist));
  struct sl_node * ptr = to_return->blank;
  while(capacity>0){
    ptr->next = (struct sl_node *)(ptr + sizeof(struct sl_node));
    capacity--;
  }
  ptr->next = NULL;
  return to_return;
}

struct slist *sl_expand(struct slist *sl,
			void * (* my_alloc)(size_t),
			size_t delta)
{
  void * p = my_alloc(delta * sizeof(struct sl_node));
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
  ptr = my_alloc(sizeof(struct sl_node));
  ptr->data = p;
  ptr->next = sl->memlist;
  sl->memlist = ptr;
  return sl;
}

void sl_free(void (* my_free) (void *),
	     struct slist *sl)
{
  struct sl_node * ptr;
  struct sl_node * ptr2;

  ptr = sl->memlist; /* hold the memlist */
  while(ptr != NULL){
    ptr2 = ptr->next; /* hold the next pointer */
    my_free(ptr->data);
    my_free(ptr);
    ptr = ptr2;
  }
}

int append(void * data, struct slist * sl)
{
  if(unlikely(sl->length == sl->capacity))
    return 1;
  else{
    struct sl_node * ptr = sl->blank;
    sl->blank = sl->blank->next;
    ptr->data = data;
    if(unlikely(sl->head == NULL)){
      sl->head = ptr;
      sl->end = ptr;
    }else{
      sl->end->next = ptr;
      sl->end = ptr;
    }
    sl->length++;
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
  sl->length--;
  return to_return;
}

int push(void * data, struct slist *sl)
{
  if(unlikely(sl->length == sl->capacity))
    return 1;
  else{
    struct sl_node * ptr = sl->blank;
    sl->blank = sl->blank->next;
    ptr->data = data;
    ptr->next = sl->head;
    sl->head = ptr;
    if(unlikely(sl->end == NULL))
      sl->end = ptr;
    sl->length++;
    return 0;
  }
}

struct sl_node * find(void * data,struct slist * sl){
	struct sl_node * ptr;
	ptr = sl->head;
	while(ptr){
		if(ptr->data == data)
			return ptr;
		ptr=ptr->next;	
	}
	return NULL;
}

int mv2head(void * data,struct slist * sl){
	struct sl_node * cur, * pre;
	cur = sl->head;
	if(cur->data == data)
		return 0;
	pre = cur;
	cur = cur->next;
	while(cur){
		if(cur->data == data){
			if(cur == sl->end)
				sl->end = pre;
			pre->next = cur->next;
			cur = sl->head;
			sl->head = cur;
			return 0;
		}
		pre = cur;
		cur = cur->next;
		return 0;
	}
	return 1;	
}

void traverse(void (* visit)(struct sl_node *),
	      struct slist *sl)
{
  struct sl_node * ptr;
  ptr = sl->head;
  while(ptr != NULL){
    visit(ptr);
    ptr = ptr->next;
  }
}
