
DESTDIR = ./bin

TEST = test
TARGETA = dnsrequest
TARGETB = libwraps.a


CC = gcc

#for debug
CFLAGS =  -I../include/ -L./lib -g -Wall 
#for release
#CFLAGS = -o -ansi  


all: $(TEST) $(TARGETB)

$(TARGETA).o:$(TARGETA).c
	$(CC) $(CFLAGS) -c  $^

$(TARGETB): wraps.c errors.c
	gcc  $(CFLAGS) -c $^
	ar rs $@ wraps.o errors.o
	mv $@ ./lib

$(TEST).o: $(TEST).c
	$(CC) $(CFLAGS) -c $^
$(TEST): $(TARGETA).o $(TEST).o
	$(CC) $(CLAGS) -o $@  $^ 
	mv $@ $(DESTDIR)


	
.PHONY:clean
clean:
	rm *.o  *.a

#all:dc_server
