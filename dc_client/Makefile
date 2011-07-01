libdc_client.a:dc_client.o
	ar rs libdc_client.a dc_client.o
	ranlib -t libdc_client.a
	cp libdc_client.a ../lib
dc_client.o:dc_client.c
	cc -c -g dc_client.c -L../lib -ldc_common -I../include
.PHONY:clean
clean:
	-rm *.o *.a
CPPFLAGS = -I../include
CFLAGS= -g
