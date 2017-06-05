all: symbolic.o funCalls.o

    
funCalls.o: funCalls.c
	mkdir -p ${BUILD}
	mkdir -p ${BUILD}/symbolic
	${CLANG} ${FLAGS} -c -m32 -o ${BUILD}/$@ $^
	${CLANG} ${FLAGS} -c -m32 -o ${BUILD}/symbolic/$@ $^
 
symbolic.o: symbolic.c
	mkdir -p ${BUILD}
	mkdir -p ${BUILD}/symbolic
	${CLANG} ${FLAGS} -c  -m32 -o ${BUILD}/$@ $^
	${CLANG} ${FLAGS} -c -D NO_SYMBOLIZE  -m32 -o ${BUILD}/symbolic/$@ $^

.PHONY : clean
clean: 
	rm -r ${BUILD}
