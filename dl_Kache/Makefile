all:libdl_kache.a list2tri test_tri
libdl_kache.a:dl_kache.o alpha-map.o darray.o fileutils.o tail.o trie.o dc_mm.o
	ar rs dl_kache.o alpha-map.o darray.o fileutils.o tail.o trie.o dc_mm.o
	ranlib -t libdl_kache.a
test_tri: test_tri.c alpha-map.o darray.o fileutils.o tail.o trie.o
	${CC} -o test_tri test_tri.c alpha-map.o darray.o fileutils.o tail.o trie.o
list2tri: list2tri.c alpha-map.o darray.o fileutils.o tail.o trie.o
	${CC} -o list2tri list2tri.c alpha-map.o darray.o fileutils.o tail.o trie.o
.c:.o
	${CC} -c %<
.PHONY:clean
clean:
	-rm *.o list2tri test_tri
CC=cc -g