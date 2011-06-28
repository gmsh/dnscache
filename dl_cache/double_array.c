/*
 * double_array.c
 *
 *  Created on: Jun 26, 2011
 *      Author: gmsh
 */

#include "double_arrary.h"
#include "dc_mm.h"
#incldue "dc_set.h"

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
};

static inline code next_code(uint8 * key,
			     int index)
{
  return get_code(*(key + index++));   
}

static inline enum cell_state next_state(state s,
					 code _next_code, double_arrary * da)
{
  /* TODO use mask to implement */
  state t = da->cells[s].base + _next_code;
  if(unlikely(t > da->cell_nums))
    return overflow;
  if(da->cells[t].check == s){
    if (da->cells[t].base < 0)
      return in_tail;
    else
      return in_da;
  }else{
    if(da->cells[t].check < 0){
      if(unlikely(da->cells[t].base > 0))
	return invalid;
      else
	return idle;
    }else{
      if(unlikely(da->cells[t].base < 0))
	return invalid;
      else
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
  
  return to_return;
}

static void expand_double_array(double_arrary * da)
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

void da_insert(uint8 * key, void * data, 
	       double_array * da)
{
  int32 index = 0;
  int32 state = ROOT_STATE;
  code c;
  do{
    c = next_code(key, index);
    switch(c){
    case overflow:
      break;
    case occupied_by_othre:
      break;
    case in_da:
      break;
    case in_tail:
      break;
    case invalid:
      return;
    default:
      return;
    }
  }while(END_CODE != c);
   
}

