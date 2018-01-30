all: symbolic.o funCalls.o

    
funCalls.o: funCalls.c
	mkdir -p ${BUILD}
	mkdir -p ${BUILD}/symbolic
	mkdir -p ${BUILD}/wd
	${CLANG} ${FLAGS} -c -m32 -o ${BUILD}/$@ $^
	${CLANG} ${FLAGS} -c -m32 -o ${BUILD}/symbolic/$@ $^
	${CLANG} ${FLAGS} -c -m32 -o ${BUILD}/wd/$@ $^
 
symbolic.o: symbolic.c
	mkdir -p ${BUILD}
	mkdir -p ${BUILD}/symbolic
	mkdir -p ${BUILD}/wd
	${CLANG} ${FLAGS} -c  -m32 -o ${BUILD}/$@ $^
	${CLANG} ${FLAGS} -c -D NO_SYMBOLIZE  -m32 -o ${BUILD}/symbolic/$@ $^
	${CLANG} ${FLAGS} -c -DNO_SYMBOLIZE -DLOWEST_SOLUTION -m32 -o ${BUILD}/wd/$@ $^

.PHONY : clean
clean: 
	rm -r ${BUILD}
