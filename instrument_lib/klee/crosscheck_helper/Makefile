all: lib_produce.o

lib_produce.o: lib_produce.c
	${CLANG} -c -o $@ $^


.PHONY : clean
clean: 
	rm  -r lib_produce.o
