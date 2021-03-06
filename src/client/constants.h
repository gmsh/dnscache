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
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <pthread.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>

#define MAXLINE		4096	// normal buffer size
#define ARRAYSIZE	5024000	// array for the dns threads
#define  LINES     	1843314	// lines read from the domain.list

//1843314


#define  DNS_ERROR	"127.0.0.1"
#define	 CACHE_MISS	"127.0.0.2"
#define  FIRST_WITH_ERROR	0x3f	//0011 1111  0 with error ,
					//           1 without error
#define  FIRST_WITHOUT_ERROR 	0x7f	//0111 1111
#define	 SECOND_WITH_ERROR	0xbf	//1011 1111
#define	 SECOND_WITHOUT_ERROR	0xff	//1111 1111



#define TOTAL_LENGTH		4	// total length of the messages
#define MAGIC_NUMBER_LENGTH	4	// length of the MAGICNUMBER
#define MAGIC_NUMBER		0x12345678	// magic number
#define REQUEST_NUMBER_LENGTH	4	// length of the request NO.
#define	RESERVED_BYTE_LENGTH	1	// length of the reserved byte in
						// request message
#define HEAD_LENGTH		(TOTAL_LENGTH + MAGIC_NUMBER_LENGTH +\
				REQUEST_NUMBER_LENGTH + RESERVED_BYTE_LENGTH )
#define SERIAL_NUMBER_MASK	0x80	// 0 means first return message.
						// 1 means second return messag
#define	ERROR_CONTRL_MASK	0x40	// 0 means no errors in the
						// message ,if this is in the
						// first return message, it also
						// means no second message
						// 1 means there are dns
						// requests, results will be in
						// the second message.
#define	RESERVED_BYTE 	'F'
#define	PORT	8888		//server port
#define SERVIP	"211.87.232.119"

//modify 
#define MAXDNSTHREADS   50	/* the number of DNS threads  */
#define MAXSERVTHREADS  1000	/* the number of work threads */

#define MAXLISTENQ	4096

#define TIME_OUT	86400	/* 24 hours in seconds */

#define TCP_OR_UDP 	2048
//synckey

/* wakemecn: used by dns cache memory management modules. */
#define SMALL 1			/* the 2^SMALL is the smallese chunks */
#define	BIG	20		/* the 2^BIG is the biggest chunks */
#define CHUNK_TYPE_NUM ( BIG - SMALL + 1 )	/* counts of  chunk's type */
#define DEFAULT_EXTRA 2		/* default number of each type of extra chunks  */
#define BIGGEST_CAP 30
/* wakemecn: end */

#endif /* CONSTANTS_H_ */
