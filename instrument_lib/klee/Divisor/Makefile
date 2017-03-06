all: symbolic.bc funCalls.bc

    
funCalls.bc: funCalls.c
	mkdir -p ${BUILD}
	${CLANG} -c -emit-llvm -o ${BUILD}/$@ $^
 
symbolic.bc: symbolic.c sieve.o
	mkdir -p ${BUILD}
#	${BUILD}/sieve.o 10479 > primes.c
	${BUILD}/sieve.o  > primes.c
	${CLANG} -c -emit-llvm -o ${BUILD}/$@ $<

sieve.o: sieve.c
	mkdir -p ${BUILD}
	${CLANG} sieve.c -o ${BUILD}/sieve.o -lm

.PHONY : clean
clean: 
	rm  ${BUILD}/*.bc
