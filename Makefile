all:
	$(MAKE) -C instrument_lib   
	$(MAKE) -C scripts
	$(MAKE) -C src2src

