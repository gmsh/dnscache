
DESTDIR = ./bin

TEST = test
TESTC = testclient
TARGETA = dnsrequest
TARGETB = libwraps.a
TARGETC = thread_make.o 


CC = gcc

#for debug
CFLAGS =  -I../include/  -g -Wall 
LIB = -Llib -lwraps -lpthread
#for release
#CFLAGS = -o -ansi  


all: $(TEST) $(TARGETB) $(TARGETC) $(TESTC) 

$(TARGETA).o:$(TARGETA).c 
	$(CC) $(CFLAGS) -c  $^  $(LIB)

$(TARGETB): wraps.c errors.c
	gcc  $(CFLAGS) -c $^
	ar rs $@ wraps.o errors.o
	cp $@ ./lib

$(TARGETC):thread_make.c $(TARGETA).o 
	gcc $(CFLAGS) -c  $^	$(LIB)


$(TEST).o: $(TEST).c
	$(CC) $(CFLAGS) -c $^
$(TEST): $(TARGETA).o $(TEST).o  $(TARGETC)
	$(CC) $(CLAGS) -o $@  $^ $(LIB)
	mv $@ $(DESTDIR)

$(TESTC).o: $(TESTC).c
	$(CC) $(CFLAGS) -c $^
$(TESTC): $(TESTC).o  
	$(CC) $(CLAGS) -o $@  $^ $(LIB)
	mv $@ $(DESTDIR)


	
.PHONY:clean
clean:
	rm *.o  *.a

#all:dc_server
