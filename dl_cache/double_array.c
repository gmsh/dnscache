/*
 * double_array.c
 *
 *  Created on: Jun 26, 2011
 *      Author: gmsh
 */

#include "double_arrary.h"
#include "dc_mm.h"
typedef struct _da_cell {
	int32 base;
	int32 check;
	void * user_data;
} da_cell;

struct _dobule_array {
	/*
	 *	In  order to  keep a  node  number  from  exceeding the
	 *  maximum index of the double-array,  we define the  size,
	 *	denoted by cell_nums.
	 */
	int32 cell_num;
	da_cell * cells;
};

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

