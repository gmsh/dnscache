.PHONY:all
all:test_client dns_show
	cp $^ ../../bin
	cp domain.list ../../bin
test_client: test_client.o dc_client.o datrie_tail_pool.o dc_mm.o slist.o 
	${CC}  -o $@ $^ -lpthread
dns_show: dns_show.o dc_client.o
	${CC}  -o $@ $^

.c:.o
	${CC} -c %<
.PHONY:clean
clean:
	-rm *.o  test_client dns_show
CC=cc -O2 -I../
