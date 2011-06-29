
DESTDIR = ./bin

TEST = test
TESTC = testclient
TESTD = servtest
TARGETA = dnsrequest
TARGETB = libwraps.a
TARGETC = dns_thread_make.o
TARGETD = moveport
TARGETE = mainserver
TARGETF = serv_thread_make.o
TARGETG = init.o


CC = gcc

#for debug
CFLAGS =  -I../include/  -g -Wall 
LIB = -Llib -L../lib -lwraps -lpthread  -ldc_mm  -ldl_cache
#for release
#CFLAGS = -o -ansi  


all: $(TARGETB) $(TARGETC) $(TESTC)  $(TARGETD) $(TARGETE) $(TESTD) $(TEST)

$(TARGETA).o:$(TARGETA).c 
	$(CC) $(CFLAGS) -c  $^  $(LIB)

$(TARGETB): wraps.c errors.c
	gcc  $(CFLAGS) -c $^
	ar rs $@ wraps.o errors.o
	cp $@ ./lib

$(TARGETC): dns_thread_make.c $(TARGETA).o 
	$(CC) $(CFLAGS) -c  $^	$(LIB)

$(TARGETD).o: $(TARGETD).c
	$(CC) $(CFLAGS) -c $(TARGETD).c

$(TARGETD): $(TARGETD).o
	$(CC) $(CFLAGS) -o $@ $^
	mv $@ $(DESTDIR)


$(TARGETE).o: $(TARGETE).c $(TARGETA).c server.h $(TARGETG)
	$(CC) $(CFLAGS) -c $^
$(TARGETE): $(TARGETE).o $(TARGETF) $(TARGETG) $(TARGETA).o $(TARGETC)
	$(CC) $(CFLAGS) -o $@ $^ $(LIB) 
	mv $@ $(DESTDIR)

$(TARGETF): serv_thread_make.c dns_thread_make.c $(TARGETB)  
	$(CC) $(CFLAGS) -c  $^	$(LIB)

$(TARGETG): init.c  
	$(CC) $(CFLAGS) -c  $^	$(LIB)


$(TEST).o: $(TEST).c $(TARGETB)
	$(CC) $(CFLAGS) -c $^
$(TEST): $(TARGETA).o $(TEST).o  $(TARGETC)
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
	rm *.o  

#all:dc_server
