/*
 * constants.h
 *
 *  Created on: Jun 26, 2011
 *      Author: gmsh
 */

#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#define DATRIE_TAIL_POOL_INITIAL_SIZE 512
#define DATRIE_TAIL_POOL_INCREASING_RATE 2
#define DOUBLE_ARRARY_INITIAL_SIZE 512
#define DOUBLE_ARRARY_INCREASING_RATE 2


//sycnkey
#define TOTAL_LENGTH		4		// total length of the messages
#define MAGIC_NUMBER_LENGTH	4		// length of the MAGICNUMBER
#define MAGIC_NUMBER		0x12345678	// magic number
#define REQUEST_NUMBER_LENGTH	4		// length of the request NO.
#define	RESERVED_BYTE_LENGTH	1		// length of the reserved byte in
						// request message
#define SERIAL_NUMBER_MASK	0x80		// 0 means first return message.
						// 1 means second return messag
#define	ERROR_CONTRL_MASK	0x40		// 0 means no errors in the
						// message ,if this is in the
						// first return message, it also
						// means no second message
						// 1 means there are dns
						// requests, results will be in
						// the second message.
#define	PORT	8888				//server port

//modify 
#define MAXDNSTHREADS   20  			/* the number of DNS threads  */
#define MAXSERVTHREADS  200    			/* the number of work threads */


//synckey

#endif /* CONSTANTS_H_ */
