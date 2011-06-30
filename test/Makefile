.PHONY: test default
default:clean test_double_array
test: test_dc_set test_datrie_tail_pool test_double_array
test_typedefs:test_typedefs.c

test_dc_set:test_dc_set.c
	${CC} ${CFLAGS} ${CPPFLAGS} -o test_dc_set test_dc_set.c -L../lib -ldl_cache

test_datrie_tail_pool:test_datrie_tail_pool.c
	${CC} ${CFLAGS} ${CPPFLAGS} -o test_datrie_tail_pool test_datrie_tail_pool.c -L../lib -ldl_cache

test_double_array:test_double_array.c
	make -C ../dl_cache clean
	make -C ../dl_cache
	${CC} ${CFLAGS} ${CPPFLAGS} -o test_double_array test_double_array.c -L../lib -ldl_cache

.PHONY: clean
clean:
	-${RM} test_typedefs test_dc_set test_datrie_tail_pool test_double_array
CC = gcc
CFLAGS = -g 
CPPFLAGS = -I../include
RM = rm -f