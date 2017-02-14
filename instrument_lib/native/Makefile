all: symbolic.o funCalls.o symbolic.bc funCalls.bc
	$(MAKE) -C modeC

    
funCalls.o: funCalls.c
	mkdir -p ${BUILD}
	mkdir -p ${BUILD}/fuzzballModeA
	${CLANG} -c -o ${BUILD}/$@ $^
	${CLANG} -c -m32 -o ${BUILD}/fuzzballModeA/$@ $^

funCalls.bc: funCalls.c
	mkdir -p ${BUILD}/kleeModeA
	${CLANG} -c -emit-llvm -o ${BUILD}/kleeModeA/$@ $^
 
symbolic.o: symbolic.c
	mkdir -p ${BUILD}
	mkdir -p ${BUILD}/fuzzballModeA
	${CLANG} -c -o ${BUILD}/$@ $^
	${CLANG} -c -m32 -o ${BUILD}/fuzzballModeA/$@ $^


symbolic.bc: symbolic.c
	mkdir -p ${BUILD}/kleeModeA
	${CLANG} -c -emit-llvm -o ${BUILD}/kleeModeA/$@ $^

.PHONY : clean
clean: 
	rm  -r ${BUILD}
	$(MAKE) -C modeC clean
