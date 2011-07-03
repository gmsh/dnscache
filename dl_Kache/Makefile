all:list2tri test_tri
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