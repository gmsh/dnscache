/*
 * double_array.c
 *
 *  Created on: Jun 26, 2011
 *      Author: gmsh
 */

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
