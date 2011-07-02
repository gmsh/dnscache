/*
 * double_array.c
 *
 *  Created on: Jun 26, 2011
 *      Author: gmsh
 */

#include "double_array.h"
#include "dc_mm.h"
#include "dc_set.h"
#include "datrie_tail_pool.h"
#include "constants.h"
#include "typedefs.h"
#include "dc_bitmap.h"
#include <stdio.h>
#include <stdlib.h>

typedef int32 state;
typedef uint8 code;

#define ROOT_STATE 1 /* root cell */
#define IDLE_LIST 0 /* use cell[0] as idle list */

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
  end_with_zero, /* 
		  * another key end with '\0'
		  * the key's user_data(not NULL) stores here.
		  * check > 0, base should >= 0 in case of it
		  * is in_tail.
		  */
  invalid
} _cell_state;

struct _double_array{
  /*
   *	In  order to  keep a  node  number  from  exceeding the
   *  maximum index of the double-array,  we define the  size,
   *	denoted by cell_nums.
   */
  int32 cell_num;
  da_cell * cells;
  datrie_tail_pool * tails;
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
					 double_array * da)
{
  /* TODO use mask to implement */
  uint8 input_char = *(key + offset);
  if('\0' == input_char)
    return eok;
  *_next_code = get_code(input_char);
  *_next_state = da->cells[current_state].base + *_next_code;
  if(unlikely(*_next_state > da->cell_num))
    return overflow;
  if(da->cells[*_next_state].check == current_state){
    /* check > 0*/
    if (da->cells[*_next_state].base < 0)
      return in_tail;
    else{
      /* commented because da_insert don't differentiate
       *in_da & end_with_zero.
      if(da->cells[*_next_state].user data != NULL)
      return end_with_zero;
      else*/
      return in_da;
    }
  }else{
    if(da->cells[*_next_state].check <= 0){
      if(unlikely(da->cells[*_next_state].base > 0))
	return invalid;
      else
	return idle;
    }else{/* check > 0 */
      if(unlikely(da->cells[*_next_state].base < 0))
	return occupied_by_other;
    }
  }
}

static inline void occupy_state(state s, double_array * da)
{
  state _next = -da->cells[s].check;
  state _previous = -da->cells[s].base;
  da->cells[_previous].check = -_next;
  da->cells[_next].base = -_previous;
}


double_array * new_double_array()
{
  double_array * to_return = 
    dc_alloc(sizeof(double_array));
  to_return->cell_num = DOUBLE_ARRAY_INITIAL_SIZE;
  to_return->cells =
    dc_alloc(sizeof(da_cell) * DOUBLE_ARRAY_INITIAL_SIZE);
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
  for(i = 0; i < to_return->cell_num; i++){
    to_return->cells[i].base = -(i - 1);
    to_return->cells[i].check = -(i + 1); 
    to_return->cells[i].user_data = NULL;
  }
  /*base[i] is the previous free cell*/
  /*check[i] is the next free cell*/
  to_return->cells[0].base = 
    -(to_return->cell_num - 1);
  to_return->cells[to_return->cell_num -1].check
    = 0;
  occupy_state(ROOT_STATE, to_return);
  to_return->cells[ROOT_STATE].base = 1;
  to_return->cells[ROOT_STATE].check = ROOT_STATE;
  to_return->tails = new_datrie_tail_pool();
  return to_return;
}

void free_da(double_array * da)
{
  dc_free(da->cells);
  free_datp(da->tails);
  dc_free(da);
}

static inline enum cell_state 
check_state(state s, double_array * da)
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
static inline void expand_double_array(double_array * da)
{
  int32 origin_cell_num = da->cell_num;
  da->cell_num *= DOUBLE_ARRAY_INCREASING_RATE;
  da->cells = 
    dc_realloc(da->cells, sizeof(da_cell) * da->cell_num);
  int32 i;
  /* initial the new memory  */
  for(i = origin_cell_num; i < da->cell_num; i++){
    da->cells[i].base = -(i - 1);
    da->cells[i].check = -(i + 1);
    da->cells[i].user_data = NULL;
  }
  /* combine the two part */ 
  da->cells[da->cell_num - 1].check = IDLE_LIST;
  da->cells[da->cells[IDLE_LIST].base].check
    = origin_cell_num;
  da->cells[origin_cell_num].base = 
    da->cells[IDLE_LIST].base;
  da->cells[IDLE_LIST].base =
    da->cell_num - 1;

}

/*
 * base[s] > 0
 * return a _dc_bitmap(uint64). If check[base[s] + c] = s then _dc_bitmap[s]
 * is 1, otherwise 0. 
 */
static inline _dc_bitmap _dc_bitmap_of_state(state s, double_array * da)
{
  _dc_bitmap to_return = 0;
  int8 i;
  /* 1 is the first code */
  for(i = 1; i <= MAX_CODE; i++){
    /*check[base[s] + c] = s*/
    if(da->cells[da->cells[s].base + i].check == s)
      to_return = bm_set(i, to_return);
  }
  return to_return;
}


/*
 * according given _dc_bitmap, to find a suit slot to occupy.
 * after_this > IDLE_LIST.
 * retutn a possible base value of bm.
 * return -1 if bm is empty.
 */
static inline state occupy_next_free(_dc_bitmap bm,
				     double_array * da){
  state next_idle, to_return, last_idle_to_occupy,
    _previous, _next;
  int8 _next_code = first_of_1(bm);
  if(unlikely(_next_code == -1))
    return -1;
  int8 _first_code = _next_code;

  /* ensure the return base is not negative. */
 DA_NEXT_FREE_START:
  next_idle = IDLE_LIST;
  do{
    next_idle = -(da->cells[next_idle].check);
  }while(next_idle < ROOT_STATE + _first_code && next_idle != IDLE_LIST);
  if(unlikely(next_idle == IDLE_LIST)){
    /* 
     * da needs to be expand.
     */
    expand_double_array(da);
    goto DA_NEXT_FREE_START;
  }
  
 DA_FIND_SUIT_SLOT:
  to_return = next_idle - _first_code;
  while(-1 != (_next_code = next_of_1(_next_code, bm))){
      last_idle_to_occupy 
	= da->cells[to_return].base + _next_code;
      switch(check_state(last_idle_to_occupy, da)){
      case occupied_by_other:
	/*not idle, move ahead*/
	next_idle = -(da->cells[next_idle].check);
	_next_code = _first_code;
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
  /* a slot is found.
   * occupy it.
   */
    _next_code = _first_code;
  do{
    occupy_state(to_return + _next_code, da);
  }while(-1 != (_next_code = next_of_1(_next_code, bm)));
  return to_return;
}

/* free the state */
static inline void free_state(state to_free, double_array * da)
{
  state s = to_free, temp;
  while(s != IDLE_LIST){
    s--;
    if(da->cells[s].check <= 0)
      break;
    /* break if idle */
  }
  da->cells[to_free].user_data = NULL;
  temp = -(da->cells[s].check);
  da->cells[s].check = -to_free;
  da->cells[to_free].check = -temp;
  da->cells[to_free].base = -s;
  da->cells[temp].base = -to_free;
}

/*
 * move base for state to_relocate to a new place beginning 
 * at b(dest base index).
 * bm is _dc_bitmap of to_relocate.
 */
static inline void relocate(state to_relocate, state b,
			    _dc_bitmap bm, double_array * da)
{
  /*_code is not code because -1 may return*/
  /* for each input code for state to_relocate */
  int8 _code = first_of_1(bm), _code2;
  _dc_bitmap bm2;
  while(_code != -1){
    /* mark owner */
    da->cells[b + _code].check = to_relocate;
    /* base[b + c] = base[base[s] + c] */
    state base_s_c = da->cells[to_relocate].base + _code;
    da->cells[b + _code].base 
      = da->cells[base_s_c].base;
    /* copy data.*/
    da->cells[b + _code].user_data
      = da->cells[base_s_c].user_data;
    
    
    /* 
     * the node base[s] + c is to be moved to b + c. Hence, for any
     * i for which check[i] = base[s] + c, update check[i] to b + c;
     */
    bm2 = _dc_bitmap_of_state(base_s_c, da);    
    /* for eache input code for state (base[to_relocate] + c) */
    _code2 = first_of_1(bm2);
    while(_code2 != -1){
      /* check[base[base[s] + c] + d] = b + c */
      /* check[base[base_s_c] + _code2] = b + code */
      da->cells[da->cells[base_s_c].base + _code2].check = b + _code;
    }
    /* free the cell */
    free_state(base_s_c, da);
    _code = next_of_1(_code, bm);
  }
  da->cells[to_relocate].base = b;
}

/* find a idle state, occupy and return it */
static inline state find_and_occupy(code _next_code, double_array * da){
  /* to_return - _next_code should > 0*/
  state to_return = IDLE_LIST;
 START_FIND_AND_OCCUPY:
  to_return = -(da->cells[to_return].check);
  if(to_return == IDLE_LIST){
    expand_double_array(da);
    goto START_FIND_AND_OCCUPY;
  }
  if(to_return - _next_code <= 0)
    goto START_FIND_AND_OCCUPY;
  occupy_state(to_return, da);
  return to_return;
}

/* return the offset of the first different char 
 * if the two are same, return -1;
 */
static inline uint32 str_diff_offset(uint8 * str1, uint8 * str2)
{
  uint32 to_return = 0;
  while(str1[to_return] == str2[to_return]){
    if(str1[to_return] == '\0')
      return -1;
    to_return++;
  }
  return to_return;
}

/*
 * to insert some chars in to da, but will not put them in tails 
 * pool. '\0' is not one of these chars.
 * return the last state correspond to the last char, whose base
 * is not set.
 */
static inline state da_insert_without_tail
(uint8 * str, int32 length, state where, double_array * da)
{
  uint32 offset = 0;
  code _next_code;
  state to_return;
  while(offset < length){
    /* find a idle state to occupy */
    _next_code = get_code(*(str + offset));
    to_return  = find_and_occupy(_next_code,da);
    da->cells[where].base = to_return - _next_code;
    da->cells[to_return].check = where;
    offset += 1;
    where = to_return;
  }
  return to_return;
}

void da_insert(uint8 * key, void * data, 
	       double_array * da)
{
  int32 offset = 0;
  state _current_state = ROOT_STATE, _next_state, _pre_state,
    dest_base;
  code _next_code;
  _dc_bitmap bm1, bm2, bm3;
  uint8 * tail;
  uint32 s_d_o, tail_index;
  void * tail_data;
  int8 num1, num2;    
  while(1){
    _cell_state =
      check_next_state(_current_state, key,
		       offset++, &_next_code,
		       &_next_state, da);
    switch(_cell_state){
    case in_da:
    case end_with_zero:
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
      da->cells[_next_state].user_data = data;
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
      /* origin bm of _current_state */
      bm1 = _dc_bitmap_of_state(_current_state, da);
      /* if add _next_code, form bm3*/
      bm3 = bm_set(_next_code, bm1);
      /*state da->cells[_next_state].check is in_tail?*/
      if(da->cells[da->cells[_next_state].check].base < 0){
	dest_base = occupy_next_free(bm1, da);
	relocate(_current_state, dest_base, bm1, da);
	offset--;
      }
      bm2 = _dc_bitmap_of_state(da->cells[_next_state].check, da);
      num1 = num_of_1(bm3);
      num2 = num_of_1(bm2);
      if(num1 < num2){
	/* occupy bm3 */
	dest_base = occupy_next_free(bm3, da);
	/* only relocate bm2 */
	relocate(_current_state, dest_base, bm1, da);
      }else{
	dest_base = occupy_next_free(bm2, da);
	relocate(da->cells[_next_state].check, dest_base, bm2, da);
      }
      /* after do relocate the state should be idle*/
      /* move back the input char & rechek */
      offset--;
      break;
    case eok:
      /*
       * In this scenario, we save the data
       * in the current state.
       * we don't need occupy it, because it is
       * not idle.
       */
      da->cells[_current_state].user_data = data;
      return;
    case overflow:
      /* s = next state  */
      do{
	expand_double_array(da);
      }while(_next_state > da->cell_num);
      goto _IDLE;
    case in_tail://TODO
      tail_index = -(da->cells[_next_state].base);
      tail = dt_get_tail(tail_index, da->tails);

      s_d_o = str_diff_offset(tail, key + offset);
      if(s_d_o == -1){
	/* the two are same, change the user data to data */
	da->cells[_next_state].user_data = data;
	return;
      }
      tail_data = da->cells[_next_state].user_data;
      /* _pre_state is used to wipe data*/
      _pre_state = _next_state;
      if(0 != s_d_o){
	/* insert same str into da not tail */
	_current_state = da_insert_without_tail(tail, s_d_o,
						_next_state, da);
      }else{/*completely different.*/
	_current_state = _next_state;
      }
      /* 
       * insert the remanent tail &  the remanent current key
       * into da, respectively. i.e. put the different in tail. 
       * one is key + offset + s_d_o;
       * the other is tail + s_d_o;
       * next_code1 = get_code(*(one));
       * next_code2 = get_code(*(the other)); form a _dc_bitmap and 
       * use occupy_next_free function to get base;
       */
      _dc_bitmap bm3 = 0;
      uint8 * tail1 = key + offset + s_d_o;
      code next_code1 = get_code(*(tail1));
      uint8 * tail2 = tail + s_d_o;      
      code next_code2 = get_code(*(tail2));
      /* Note that only one of the two can be '\0' 
       * so bm3 should not by empty;
       */
      if(*tail1 == '\0'){
	da->cells[_current_state].user_data = data;
	/*tail_index stores the index, we can safely change base.*/
	da->cells[_current_state].base
	  = find_and_occupy(next_code2, da);
	_next_state = da->cells[_current_state].base + next_code2;
	/*move tail2 to _next_state*/
	da->cells[_next_state].check = _current_state;
	da->cells[_next_state].base = dt_push_tail(tail2 + 1, da->tails);
	da->cells[_next_state].user_data = tail_data;
	dt_remove_tail(tail_index, da->tails);
	if(_pre_state != _current_state)/* wipe data */
	  da->cells[_pre_state].user_data = data;
	return;
      }
      /** tail1 != '\0' */
      bm3 = bm_set(next_code1, bm3);
      /*_next_state can not be end_with_zero, so *tail2 != '\0'*/
      bm3 = bm_set(next_code2, bm3);
      da->cells[_current_state].base = 
	occupy_next_free(bm3, da);
      
      /* tail1 */
      _next_state = da->cells[_current_state].base + next_code1;
      da->cells[_next_state].check
	= _current_state;
      da->cells[_next_state].base
	= -dt_push_tail(tail1 + 1, da->tails);
      da->cells[_next_state].user_data
	= data;
            
      /*_next_state can not be end_with_zero, so *tail2 != '\0'*/
      _next_state = da->cells[_current_state].base + next_code2;
      da->cells[_next_state].check
	= _current_state;
      da->cells[_next_state].base
	= -dt_push_tail(tail2 + 1, da->tails);
      da->cells[_next_state].user_data = tail_data;
      
      /* wipe data */
      da->cells[_pre_state].user_data = NULL;
      
      /* after do that remove the tail from tail pool */
      dt_remove_tail(-(da->cells[_pre_state].base), da->tails);
      return;
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
  uint8 * tail;
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
      tail = dt_get_tail(
				 -(da->cells[_current_state].base),
				 da->tails
				 );
      if(0 == strcmp(tail, key + offset))
	return da->cells[_current_state].user_data;
      else
	return NULL;
    case eok:
      /* state not changed 
       * return the data. */
      return da->cells[_current_state].user_data;
    case invalid:
    case overflow:
    case occupied_by_other:
      return NULL;
    }
  }
}

void da_delete(uint8 * key, double_array * da)
{
  /* In initial implementation, this function do nothing. */
}

void da_set_data(uint8 * key, void * data, double_array * da)
{
  da_insert(key, data, da);
}


long write_da_to_file(FILE * f, long offset, double_array * da)
{
  /* 
   * write the cell_num first
   * write cell_num * state * 2,
   * base 0, check 0, base 1, check 1,...
   * Note that we don't write the tail pool, write_datp_to_file
   * function should be called by a datrie, not double_array.
   * so when we read we don't set tails, either.
   */
  if(NULL == f)
    return -1;
  size_t temp = fseek(f, offset, SEEK_SET);
  if(unlikely(temp == 0)){
    return -1;
  }
  temp = fwrite(&da->cell_num, sizeof(uint32), 1, f);
  if(unlikely(temp != 1))
    return -1;
  
  uint32 i;
  for(i = 0; i < da->cell_num; i++){
    temp = fwrite(&da->cells[i].base, sizeof(state), 1, f);
    temp += fwrite(&da->cells[i].check, sizeof(state), 1, f);
    if(unlikely(temp != 2))
      return -1;
  }
  return (1 + 2 * da->cell_num) * sizeof(uint32);
}

double_array * read_da_from_file(FILE * f, long offset)
{
  /* Note that when we have read we don't set tails */
  double_array * to_return = dc_alloc(sizeof(double_array));
  
  if(NULL == f)
    return NULL;
  size_t temp = fseek(f, offset, SEEK_SET);
  if(unlikely(temp == 0)){
    return NULL;
  }
  temp = fread(&to_return->cell_num, sizeof(uint32), 1, f);
  if(unlikely(temp != 1)){
    return NULL;
  }
  to_return->cells
    = (da_cell *)dc_alloc(sizeof(da_cell) * to_return->cell_num);
  uint32 i;
  for(i = 0; i < to_return->cell_num; i++){
    temp = fread(&to_return->cells[i].base, sizeof(state), 1, f);
    temp += fwrite(&to_return->cells[i].check, sizeof(state), 1, f);
    if(unlikely(temp != 2))
      return NULL;
  }
  return to_return;
}
