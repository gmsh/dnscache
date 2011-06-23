.PHONY:	all lirsstack test clean remove_temp
all:lirsstack test
lirsstack:
	$(MAKE) -C lirsstack
test:
	$(MAKE) -C test
clean:
	$(MAKE) -C lirsstack clean
	$(MAKE) -C test clean
remove_temp:
	find . -name *~|xargs rm