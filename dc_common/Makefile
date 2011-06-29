libdc_common.a:slist.o
	ar rs libdc_common.a slist.o
	ranlib -t libdc_common.a
	cp libdc_common.a ../lib
slist.o:slist.c
	cc -g -c slist.c -I../include
.PHONY:clean
clean:
	-rm *.o *.a
CPPFLAGS = -I../include
