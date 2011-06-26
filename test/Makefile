test: test_typedefs test_dc_set test_slist test_sizeof_ptr
test_typedefs:test_typedefs.c
test_dc_set:test_dc_set.c
	cc -o test_dc_set test_dc_set.c -I../include -L../lib -ldl_cache
test_slist:test_slist.c
	cc -o test_slist test_slist.c -I../include -L../lib -ldc_common
test_sizeof_ptr:test_sizeof_ptr.c
.PHONY: clean
clean:
	-rm test_typedefs test_dc_set test_slist test_sizeof_ptr
CPPFLAGS = -I../include
