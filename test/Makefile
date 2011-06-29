test:  test_slist test_mm 
#test_typedefs test_dc_set test_slist  test_sizeof_ptr test_datrie_tail_pool
test_typedefs:test_typedefs.c
#test_dc_set:test_dc_set.c
#	cc -o test_dc_set test_dc_set.c -I../include -L../lib -ldl_cache
test_slist:test_slist.c
	cc -g -o test_slist test_slist.c -I../include -L../lib -ldc_common
#test_sizeof_ptr:test_sizeof_ptr.c
#test_datrie_tail_pool:test_datrie_tail_pool.c
#	cc -g -o test_datrie_tail_pool test_datrie_tail_pool.c -I../include -L../lib -ldl_cache
test_mm:test_mm.c
	cc -g -o test_mm test_mm.c -I../include -L../lib -ldc_mm -ldc_common
.PHONY: clean
clean:
	-rm test_typedefs test_slist test_mm# test_sizeof_ptr test_datrie_tail_pool test_dc_set
CPPFLAGS = -I../include
CFLAGS = -g
