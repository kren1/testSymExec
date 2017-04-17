all: symbolic.o funCalls.o

    
funCalls.o: funCalls.c
	mkdir -p ${BUILD}
	${CLANG} ${FLAGS} -c -m32 -o ${BUILD}/$@ $^
 
symbolic.o: symbolic.c
	mkdir -p ${BUILD}
	${CLANG} ${FLAGS} -c  -m32 -o ${BUILD}/$@ $^

.PHONY : clean
clean: 
	rm  ${BUILD}/*.o
