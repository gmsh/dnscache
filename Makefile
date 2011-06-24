.PHONY:	all dc_common.a dc_mm.a dl_cache.a dc_server dc_client test clean remove_temp
all:dc_common.a dc_mm.a dl_cache.a dc_server dc_client test doc
dc_common.a:
	$(MAKE) -C dc_common
dc_mm.a:
	$(MAKE) -C dc_mm
dl_cache.a:
	$(MAKE) -C dl_cache
dc_server:
	$(MAKE) -C dc_server
dc_client:
	$(MAKE) -C dc_client
test:
	$(MAKE) -C test
doc:
	$(MAKE) -C doc
clean:
	$(MAKE) -C dc_common clean
	$(MAKE) -C test clean
remove_temp:
	find . -name *~|xargs rm