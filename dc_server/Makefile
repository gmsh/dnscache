
DESTDIR = ./bin

TEST = test
TARGETA = dnsrequest


CC = gcc

#for debug
CFLAGS =  -I../include/ -g -Wall 
#for release
#CFLAGS = -o -ansi  


all: $(TEST)

$(TARGETA).o:$(TARGETA).c
	$(CC) $(CFLAGS) -c  $^
	

$(TEST).o: $(TEST).c
	$(CC) $(CFLAGS) -c $^
$(TEST): $(TARGETA).o $(TEST).o
	$(CC) $(CLAGS) -o $@  $^ 
	mv $@ $(DESTDIR)


	
.PHONY:clean
clean:
	rm *.o 

#all:dc_server
