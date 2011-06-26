/*
 * datrie.c
 *
 *  Created on: Jun 26, 2011
 *      Author: gmsh
 */


#include "datrie.h"
struct _da_cell{
	uint32 base;
	uint32 check;
	void * user_data;
};

struct _dobule_array{
	uint32 cell_num;
	da_cell * cells;
};

struct _tail_block{

};

struct _tail{

};
