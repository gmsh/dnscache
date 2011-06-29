libdl_cache.a:dc_set.o datrie_tail_pool.o dc_mm.o #double_array.o
	ar rs libdl_cache.a dc_set.o datrie_tail_pool.o dc_mm.o #double_array.o
	ranlib -t libdl_cache.a
	cp libdl_cache.a ../lib
.PHONY:clean
clean:
	-rm *.o *.a
CPPFLAGS = -I../include
CFLAGS= -g
