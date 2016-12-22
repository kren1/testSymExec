/*
To be linked with the file produced when usint Intrumentation.so -funcalls or -symglob for native runs
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>

void klee_silent_exit(int code) {
    exit(0);
}

//void symbolize_and_constrain_s(uint8_t *var, int size, int64_t value, char* name) {
//    klee_make_symbolic(var, size, name);
//	int64_t var_value;
//    switch(size)
//    {
//        case 1: var_value = *(int8_t*)var; break;
//        case 2: var_value = *(int16_t*)var; break;
//        case 4: var_value = *(int32_t*)var; break;
//        case 8: var_value = *(int64_t*)var; break;
//    }
//   
//    if( var_value < value) {
//        printf("var_value %i < %i value %s\n", var_value, value, name);
//        klee_silent_exit(0);
//    }
//  
//    if(var_value > value ) {
//        printf("var_value %i > %i value %s\n", var_value, value, name);
//        klee_silent_exit(0);
//    }
//}

void symbolize_and_constrain_u(void *var, int size, uint64_t value, char* name) {
	uint64_t var_value;
    switch(size)
    {
        case 1: __CrestUChar((uint8_t*)var); var_value = *(uint8_t*)var; break;
        case 2: __CrestUShort((uint16_t*)var); var_value = *(uint16_t*)var; break;
        case 4: __CrestUInt((uint32_t*)var); var_value = *(uint32_t*)var; break;
        case 8: return;
    }

    if( var_value < value) {
//        printf("var_value %d < %d value %s\n", var_value, value, name);
        klee_silent_exit(0);
    }
  
    if(var_value > value ) {
//        printf("var_value %d > %d value %s\n", var_value, value, name);
        klee_silent_exit(0);
    }
  
}
void print_symbolic(const char* name, int64_t *val, char size)
{
    switch(size)
    {
        case 8: printf("%s: %d\n",name,*(int8_t*)val); break;
        case 16: printf("%s: %d\n",name,*(int16_t*)val); break;
        case 32: printf("%s: %d\n",name,*(int32_t*)val); break;
        case 64: printf("%s: %ld\n",name,*val); break;
    }
}
