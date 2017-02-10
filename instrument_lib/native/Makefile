all: symbolic.o funCalls.o

    
funCalls.o: funCalls.c
	mkdir -p ${BUILD}
	${CLANG} -c -o ${BUILD}/$@ $^
 
symbolic.o: symbolic.c
	mkdir -p ${BUILD}
	${CLANG} -c -o ${BUILD}/$@ $^

.PHONY : clean
clean: 
	rm  ${BUILD}/*.o
