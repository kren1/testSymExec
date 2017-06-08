all: symbolic.bc funCalls.bc wd/symbolic.bc path/symbolic.bc

    
funCalls.bc: funCalls.c
	mkdir -p ${BUILD}/wd
	mkdir -p ${BUILD}/path
	${CLANG} -c -emit-llvm -o ${BUILD}/$@ $^
	${CLANG} -c -emit-llvm -o ${BUILD}/wd/funCalls.bc $^
	${CLANG} -c -emit-llvm -o ${BUILD}/path/funCalls.bc $^
 
symbolic.bc: symbolic.c
	mkdir -p ${BUILD}
	${CLANG} -c -emit-llvm -o ${BUILD}/$@ $^

wd/symbolic.bc: symbolic.c
	mkdir -p ${BUILD}/wd
	${CLANG} -c -emit-llvm -D LOWEST_SOLUTION -o ${BUILD}/wd/symbolic.bc $^

path/symbolic.bc: symbolic.c
	mkdir -p ${BUILD}/path
	${CLANG} -c -emit-llvm -D  WRITE_PATH -o ${BUILD}/path/symbolic.bc $^


.PHONY : clean
clean: 
	rm  ${BUILD}/*.bc
