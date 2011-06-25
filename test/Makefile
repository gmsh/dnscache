test: test_typedefs test_dc_set
test_typedefs:test_typedefs.c
test_dc_set:test_dc_set.c
	cc -o test_dc_set test_dc_set.c -I../include -L../lib -ldl_cache
.PHONY: clean
clean:
	-rm test_typedefs test_dc_set
CPPFLAGS = -I../include