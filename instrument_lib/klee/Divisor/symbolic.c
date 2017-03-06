/*
To be linked with the file produced when usint Intrumentation.so -funcalls or -symglob for native runs
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>

#define N_PRIMES 10000
#include "primes.c"

void klee_silent_exit(int i)
{
//    exit(i);
}
void klee_make_symbolic(void* x, int y, char* name)
{
    return;
}
void symbolize_and_constrain_s(uint8_t *var, int size, int64_t value, char* name) {
    //unused
    exit(0);
}

void symbolize_and_constrain_u(void *var, int size, uint64_t value, char* name) {
    if(value == 0) return;
//    printf("inital %s %llu\n",name, value);
    klee_make_symbolic(var, size, name);
    
	uint64_t var_value;
    switch(size)
    {
        case 1: var_value = *(uint8_t*)var; break;
        case 2: var_value = *(uint16_t*)var; break;
        case 4: var_value = *(uint32_t*)var; break;
        case 8: var_value = *(uint64_t*)var; break;
    }
    
    if(var_value < 1 || var_value > value) klee_silent_exit(0);
    
    for(int *p = primes; *p != primes[N_PRIMES - 1]; p++)  {
        if( value == 1) return;
        unsigned d = *p;
        unsigned divisor = 1;
        //Gets the highest power of the prime, that divides value
        while( value % d == 0) {
            divisor = d;
            d= (*p) * d;
        }
        //If divisor is greater than 1, we want to use it for constraining.
        if( divisor > 1) {
//            printf("value: %llu, divisor: %d\n",  value, divisor);
            if(var_value % divisor != 0) klee_silent_exit(0);
            var_value = var_value / divisor;
        }
        value = value / divisor;
    }
    // If we reached this point the number of primes was insufficent, so we just use a simple equals constrainer
//    printf("final val %llu\n", value);
    if(var_value != value) klee_silent_exit(0);
}
void print_symbolic(const char* name, int64_t *val, char size)
{
    switch(size)
    {
        case 8: printf("%s: %d\n",name,*(int8_t*)val); break;
        case 16: printf("%s: %d\n",name,*(int16_t*)val); break;
        case 32: printf("%s: %d\n",name,*(int32_t*)val); break;
        case 64: printf("%s: %lld\n",name,*val); break;
    }
}
