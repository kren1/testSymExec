all: symbolic.bc funCalls.bc

    
funCalls.bc: funCalls.c
	mkdir -p ${BUILD}
	${CLANG} -c -emit-llvm -o ${BUILD}/$@ $^
 
symbolic.bc: symbolic.c
	mkdir -p ${BUILD}
	${CLANG} -c -emit-llvm -o ${BUILD}/$@ $^

.PHONY : clean
clean: 
	rm  ${BUILD}/*.bc
