CLANG := clang-3.4
BUILD := build
FLAGS := -I$(shell pwd)/common -E
#FLAGS := -I$(shell pwd)/common 

export CLANG BUILD FLAGS

all:
	$(MAKE) -C klee
	$(MAKE) -C native
	$(MAKE) -C fuzzball

.PHONY: clean 
clean:
	$(MAKE) -C klee clean
	$(MAKE) -C native clean
	$(MAKE) -C fuzzball clean
