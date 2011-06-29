/*
 * double_array.c
 *
 *  Created on: Jun 26, 2011
 *      Author: gmsh
 */

#include "double_arrary.h"
#include "dc_mm.h"
#include "dc_set.h"
#include "datrie_tail_pool.h"

typedef int32 state;
typedef uint8 code;

#define likely(x)       __builtin_expect((x),1)
#define unlikely(x)     __builtin_expect((x),0)
#define ROOT_STATE 1 /* root cell */
#define IDLE_LIST 0 /* use cell[0] as idle list */
#define END_CODE get_code('\0')

typedef struct _da_cell {
  /*
   * state is int32 actually.
   * base&check could be used as a int64.
   * x86_64 is little edian.
   * int32: 0xdcba (d, c, b, a is a byte).
   * int64: a1b1c1d1a2b2c2d2 (in memory).
   *        0xd2c2b2a2d1c1b1a1.
   * int64 base_check = *((int64*)(&(base)))
   * int64 mask = base_check&0x8000000080000000
   * switch(mask){
   *   case 8000000080000000:
   *     check < 0;
   *     base < 0;
   *   case 8000000000000000:
   *     check < 0;
   *     base > 0;
   *   case 0000000080000000:
   *     base > 0;
   *     check < 0;
   *   case 0000000000000000:
   *     base > 0;
   *     check > 0;
   * }
   */
  state base;
  state check;
  void * user_data;
} da_cell;

static enum cell_state{
  overflow,
  occupied_by_other,
  idle,
  in_tail, /* the remanent is in tail pool, that is a tail*/
  in_da, /* the remanent is in double_array */
  eok, /* end of the key */
  ininvalid
} _cell_state;

struct _dobule_array {
  /*
   *	In  order to  keep a  node  number  from  exceeding the
   *  maximum index of the double-array,  we define the  size,
   *	denoted by cell_nums.
   */
  int32 cell_num;
  da_cell * cells;
  datire_tail_pool * tails;
};

/*
 * check the next to the current state by input char *(key
 * + offset).
 * Note that if next state is invalid the _next_state
 * will be set but useless.
 * On another condition, next state is overflowed, the 
 * _next_state and _next_code are set but the double array
 * need to expand.
 * _next_code and _next_state will not be set if the input
 * char is '\0';
 */
static inline enum cell_state check_next_state(state current_state,
					 uint8 * key,
					 int offset,
					 code * _next_code,
					 state * _next_state,
					 double_arrary * da)
{
  /* TODO use mask to implement */
  uint8 input_char = *(key + offset);
  if('\0' == input_char)
    return eok;
  *_next_code = get_code(*(input_char));
  *_next_state = da->cells[current_state].base + *_next_code;
  if(unlikely(*_next_state > da->cell_nums))
    return overflow;
  if(da->cells[_next_state].check == s){
    if (da->cells[_next_state].base < 0)
      return in_tail;
    else
      return in_da;
  }else{
    if(da->cells[_next_state].check < 0){
      if(unlikely(da->cells[_next_state].base > 0))
	return invalid;
      else
	return idle;
    }else{
      if(unlikely(da->cells[_next_state].base < 0))
	return occupied_by_other;
    }
  }
}


double_array * new_double_array()
{
  double_array * to_return = 
    dc_alloc(sizeof(double_array));
  to_return->cell_num = DOUBLE_ARRARY_INITIAL_SIZE;
  to_return->cells =
    dc_alloc(sizeof(dc_cell) * DOUBLE_ARRARY_INITIAL_SIZE);
  /*	In order to manage the free cells, let r1 r2 r3 ...
   *	rm be the free cells in the double-array structure,
   *	ordered by position.
   *		check[0] = -r1
   *		check[ri] = -r( i + 1 )
   *		1 <=i <= m -1
   *		check[rm] = 0;
   *		base[0] = -rm;
   *		base[r1] = -r0;
   *		base[ri] = -r( i - 1)
   */  
  int32 i;
  for(i = 0; i < to_return->cellnum; i++){
    to_return->cells[i].base = -(i - 1);
    to_return->cells[i].check = -(i + 1); 
  }
  /*base[i] is the previous free cell*/
  /*check[i] is the next free cell*/
  to_return->cells[0].base = 
    to_return->-(to_return->cellnum - 1);
  to_return->cells[to_return->cellnum -1]
    = 0;
  to_return->tails = new_datrie_tail_pool();
  return to_return;
}

static inline enum cell_state 
check_state(state s, double_arrary * da)
{
  if(s > da->cell_num)
    return overflow;
  if(da->cells[s].check < 0)
    return idle;
  /* note that it may occupied by itself,
   * so here is a waste a memory.
   */
  return occupied_by_other;
}
static inline void expand_double_array(double_arrary * da)
{
  int32 origin_cell_num = da->cell_num;
  da->cell_num *= DOUBLE_ARRARY_INCREASING_RATE;
  da->cells = 
    dc_realloc(sizeof(dc_cell) * da->cell_num);
  int32 i;
  /* initial the new memory  */
  for(i = origin_cell_num; i < da->cell_num; i++){
    da->cells[i].base = -(i - 1);
    da->cells[i].check = -(i + 1);
  }
  /* combine the two part */ 
  da->cells[da->cell_num - 1].check = IDLE_LIST;
  da->cells[origin_cell_num].base = 
    da->cells[IDLE_LIST].base;
  da->cells[IDLE_LIST].base =
    da->cells[da->cell_num - 1];
  da->cells[da->cells[IDLE_LIST]].check
    = origin_cell_num;
}

/*
 * return a bitmap(uint64). If check[base[s] + c] = s then bitmap[s]
 * is 1, otherwise 0.
 */
static inline uint64 bitmap_of_state(state s, double_arrary * da)
{
  uint64 to_return = 0;
  int32 i;
  for(i = 0; i < MAX_CODE; i++){
    /*check[base[s] + c] = s*/
    if(da->cells[da->cells[s].base + i].check == s)
      return = (return&0x1) << 1;
    else
      return = return << 1;
  }
  return to_return;
}

static inline int8 num_of_1(uint64 bitmap)
{
  int8 to_return = 0, i = 0;
  while(to_return < MAX_CODE){
    if(bitmap>>i&0x1)
      to_return++;
  }
  return to_return;
}

/*after_this > IDLE_LIST*/
static inline state occupy_next_free(int32 size, state after_this){
  state to_return, _previous, _next;
  int8 i; 
  code c;
 DA_NEXT_FREE_START:
  to_return = IDLE_LIST;
  do{
    to_return = -(da->cells[to_return].check);
  }while(to_return < after_this && to_return != IDLE_LIST);
  if(unlikely(to_return == IDLE_LIST)){
    /* da needs to be expand, but i think it should never
     * happen.
     */
    expand_double_array(da);
    goto DA_NEXT_FREE_START;
  }
  
 DA_FIND_SUIT_SLOT:
  for(i = 0; i < MAX_CODE; i++){
    if((bm>>i)&0x1){
      c = da->cells[to_return].base + (MAX_CODE - i);
      switch(check_state(c, da)){
      case occupied_by_other:
	/*not idle, move ahead*/
	to_return = -(da->cells[to_return].check);
	goto DA_FIND_SUIT_SLOT;
	  break;/*useless*/
      case idle:
	/* check next code*/
	break;
      case overflow:
	  expand_double_array(da);
	  goto DA_FIND_SUIT_SLOT;
	  break; /* useless */
      }
    }
  }
  /*a slot is found.*/
  return to_return;

 /* DA_FIND_CONTINOUS_IDLE_CELLS: */
 /*  for(offset = 0; */
 /*      offset < size - 1 */
 /* 	&&  */
 /* 	da->cells[to_return + offset].check == -(to_return + offset + 1); */
 /*      offset++); */
 /*  if(offset == size - 1){ */
 /*    /\* occupy state ... state + size -1*\/ */
 /*    _previous = -(da->cells[to_return].base); */
 /*    _next = -(da->cells[to_return + size -1].check); */
 /*    da->cells[_previous].check = -_next; */
 /*    da->cells[_next].base = -_previous; */
 /*    return to_return; */
 /*  }else{ */
 /*    if(da->cells[to_return + offset -1].check == IDLE_LIST){ */
 /*      /\* da need to be expand *\/ */
 /*      expand_double_array(da); */
 /*      goto DA_FIND_CONTINOUS_IDLE_CELLS;/\*TODO*\/ */
 /*    }else{ */
 /*      /\* change to_return and find again.  *\/ */
 /*      to_return = da->cells[to_return + offset - 1]; */
 /*      goto DA_FIND_CONTINOUS_IDLE_CELLS; */
 /*    } */
 /*  } */
}

static inline void relocate(state s, uint64 bm, double_array * da)
{
  state b = 
}

static inline void occupy_state(state s, double_array *)
{
  state _next = -da->cells[s].check;
  state _previous = -da->cells[s].base;
  da->cells[_previous].check = -_next;
  da->cells[_next].base = -_previous;
}

void da_insert(uint8 * key, void * data, 
	       double_array * da)
{
  int32 offset = 0;
  state _current_state = ROOT_STATE, _next_state;
  code _next_code;
  while(1){
    _cell_state =
      check_next_state(_current_state, key,
		       offset++, &_next_code,
		       &_next_state, da);
    switch(_cell_state){
    case in_da:
      /* move ahead */
      _current_state = _next_state;
      break;
    case idle:
    _IDLE:
      /*
       * Considering the following scenario, 
       * the key is abc and the input char
       *             ^
       * is b. 
       * next_state = base[current_state] + code(b)
       * and the next state cell is idle.
       * We let the check[next_state] equal to
       * current state. Then base[next_state]
       * = -dt_push_tail("c");
       * We save the data in the next state.
       */
      occupy_state(_next_state, da);
      da->cells[_next_state].check = _current_state;
      da->cells[_next_state].base =
	-(dt_push_tail(key + offset, da->tails));
      da->cells[_next_state].userdata = data.
      return;
    case occupied_by_other:
      /*
       * occupied_by_other
       * the 'other' is check[next_state].
       * Comparing the num_of_state_next_to(current_state)
       * with num_of_state_next_to(other), we relocate
       * current state if the former plus one is less than
       * the latter.
       */
      uint64 bm1 = bitmap_of_state(_current_state, da);
      uint64 bm2 = bitmap_of_state(da->cells[_next_state].check);
      int8 num1 = num_of_1(bm1);
      int8 num2 = num_of_1(bm2);
      if(num1 + 1 < num2)
	relocate(_current_state, bm1, da);
      else
	relocate(da->cells[_next_state].check, bm2, da);
      
      break;/*TODO*/
    case eok:
      /*
       * In this scenario, we save the data
       * in the current state.
       * we don't need occupy it, because it is
       * not idle.
       */
      da->cells[_current_state].userdata = data;
      return;
    case overflow:
      /* s = next state  */
      do{
	expand_double_array(da);
      }while(s > da->cell_num);
      goto _IDLE;
    case in_tail:
      /**/
      break;
    case invalid:
      return;
    default:
      return;
    }
  }
   
}

void * da_get_data(uint8 * key, double_array * da)
{
  void * to_return;
  state _current_state = ROOT_STATE, _next_state;
  code _next_code;
  int32 offset = 0;
  while(1){
    _cell_state = 
      check_next_state(_current_state, key, offset++,
		 &_next_code, &_current_state, da);
    switch(_cell_state){
    case in_da:
      /* move to next state */
      break;
    case in_tail:
      /* compare the tail & the remanent. */
      uint8 * tail = dt_get_tail(
				 -(da->cells[_current_state].base)
				 da->tails
				 );
      if(0 == strcmp(tail, key + offset))
	return da->cells[_current_state].user_data;
      else
	return NULL;
    case eok:
      /* state not changed 
       * return the data. */
      return da->cells[current_state].user_data;
    case invalid:
    case overflow:
    case occupied_by_other:
      return NULL;
    }
  }
}
