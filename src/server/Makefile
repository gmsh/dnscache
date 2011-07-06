.PHNOY:all
all:mainserver test_dl_kache list2tri test_tri 
	-cp $^ ../../bin
	-cp domains.tri ip.list ../../bin
test_dl_kache: test_dl_kache.c dl_kache.o alpha-map.o darray.o fileutils.o tail.o trie.o dc_mm.o slist.o hash.o
	${CC} -o test_dl_kache test_dl_kache.c dl_kache.o alpha-map.o darray.o fileutils.o tail.o trie.o dc_mm.o slist.o hash.o
test_tri: test_tri.c alpha-map.o darray.o fileutils.o tail.o trie.o slist.o
	${CC} -o test_tri test_tri.c alpha-map.o darray.o fileutils.o tail.o trie.o
list2tri: list2tri.c alpha-map.o darray.o fileutils.o tail.o trie.o slist.o
	${CC} -o list2tri list2tri.c alpha-map.o darray.o fileutils.o tail.o trie.o slist.o

mainserver: dnsrequest.o dns_thread_make.o errors.o mainserver.o  init.o wraps.o udp_server.o \
	 serv_thread_make.o dl_kache.o alpha-map.o darray.o fileutils.o tail.o trie.o dc_mm.o slist.o hash.o lirs.o
	${CC} -o $@ $^ -lpthread


.c:.o
	${CC} -c %<
.PHONY:clean
clean:
	-rm *.o test_dl_kache list2tri test_tri mainserver
CC=cc -O2 -w
