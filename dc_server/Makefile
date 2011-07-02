

DESTDIR = ./bin

TEST = test
TESTC = testclient
TESTD = servtest
TARGETA = dnsrequest.o
TARGETB = libserver.a
TARGETC = dns_thread_make.o
TARGETD = moveport
TARGETE = mainserver
TARGETF = serv_thread_make.o
TARGETG = init.o
TARGETH = udp_server.o


CC = gcc

#for debug
CFLAGS =  -I../include/  -g -Wall 
LIB = -Llib -L../lib -lserver -lpthread  -ldc_mm  -ldl_cache
#for release
#CFLAGS = -o -ansi  


all: $(TARGETB) $(TARGETC) $(TESTC)  $(TARGETD) $(TARGETE) $(TESTD) $(TEST)

$(TARGETA): dnsrequest.c
	$(CC) $(CFLAGS) -c  $^  

$(TARGETB): wraps.c errors.c $(TARGETC) $(TARGETF) $(TARGETA) $(TARGETH)
	gcc  $(CFLAGS) -c $^
	ar rs $@ wraps.o errors.o $(TARGETC) $(TARGETF) $(TARGETA) $(TARGETH)
	cp $@ ./lib

$(TARGETC): dns_thread_make.c 
	$(CC) $(CFLAGS) -c  $^	$(LIB)

$(TARGETD).o: $(TARGETD).c
	$(CC) $(CFLAGS) -c $(TARGETD).c

$(TARGETD): $(TARGETD).o
	$(CC) $(CFLAGS) -o $@ $^
	mv $@ $(DESTDIR)


$(TARGETE).o: $(TARGETE).c  server.h $(TARGETG)
	$(CC) $(CFLAGS) -c $^ 
$(TARGETE): $(TARGETE).o $(TARGETF) $(TARGETG) $(TARGETC)
	$(CC) $(CFLAGS) -o $@ $^ $(LIB) 
	mv $@ $(DESTDIR)

$(TARGETF): serv_thread_make.c dns_thread_make.c 
	$(CC) $(CFLAGS) -c  $^	$(LIB)

$(TARGETG): init.c  
	$(CC) $(CFLAGS) -c  $^	$(LIB)

$(TARGETH): udp_server.c 
	$(CC) $(CFLAGS) -c  $^	$(LIB)


$(TEST).o: $(TEST).c $(TARGETB)
	$(CC) $(CFLAGS) -c $^
$(TEST): $(TEST).o  $(TARGETC)
	$(CC) $(CLAGS) -o $@  $^ $(LIB)
	mv $@ $(DESTDIR)

$(TESTC).o: $(TESTC).c
	$(CC) $(CFLAGS) -c $^
$(TESTC): $(TESTC).o  
	$(CC) $(CLAGS) -o $@  $^ $(LIB)
	mv $@ $(DESTDIR)


$(TESTD).o: $(TESTD).c  server.h
	$(CC) $(CFLAGS) -c $^
$(TESTD): $(TESTD).o  
	$(CC) $(CLAGS) -o $@  $^ $(LIB)
	mv $@ $(DESTDIR)
	
.PHONY:clean
clean:
	rm *.o   *.gch

#all:dc_server
