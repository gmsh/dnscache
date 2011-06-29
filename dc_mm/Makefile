libdc_mm.a:dc_mm.o
	ar rs libdc_mm.a dc_mm.o
	ranlib -t libdc_mm.a
	cp libdc_mm.a ../lib
dc_mm.o:dc_mm.c
	cc -c -g dc_mm.c -L../lib -ldc_common -I../include
.PHONY:clean
clean:
	-rm *.o *.a
CPPFLAGS = -I../include
CFLAGS= -g
