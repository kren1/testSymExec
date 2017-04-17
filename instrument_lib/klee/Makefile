all: funCalls.bc symbolic.bc
	$(MAKE) -C modeC
	$(MAKE) -C Divisor

.PHONY: clean 
clean:
	$(MAKE) -C modeC clean
	$(MAKE) -C Divisor clean

.PHONY: mkdirs
mkdirs:
	mkdir -p LT/${BUILD}
	mkdir -p LTE/${BUILD}
	mkdir -p Range/${BUILD}

funCalls.bc: mkdirs funCalls.c ../common/constrainers/*.h
	${CLANG} ${FLAGS} -c -emit-llvm -o LT/${BUILD}/funCalls.bc funCalls.c
	${CLANG} ${FLAGS} -c -emit-llvm -o LTE/${BUILD}/funCalls.bc funCalls.c
	${CLANG} ${FLAGS} -c -emit-llvm -o Range/${BUILD}/funCalls.bc funCalls.c

 
symbolic.bc: mkdirs symbolic.c ../common/constrainers/*.h
	${CLANG} ${FLAGS} -D LT_CONSTRAIN -c -emit-llvm -o LT/${BUILD}/symbolic.bc symbolic.c
	${CLANG} ${FLAGS} -D LTE_CONSTRAIN -c -emit-llvm -o LTE/${BUILD}/symbolic.bc symbolic.c
	${CLANG} ${FLAGS} -D RANGE_CONSTRAIN -c -emit-llvm -o Range/${BUILD}/symbolic.bc symbolic.c

.PHONY : clean
clean: 
	rm -r LT/
	rm -r LTE/
	rm -r Range/
