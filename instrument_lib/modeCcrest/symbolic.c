/*
To be linked with the file produced when usint Intrumentation.so -funcalls or -symglob for native runs
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>

void symbolize_and_constrain_u(void *var, int size, uint32_t value, char* name) {
//    if(value >  1294967295) return;
	uint32_t var_value;
    switch(size)
    {
        case 1: __CrestUChar((uint8_t*)var); var_value = *(uint8_t*)var; break;
        case 2: __CrestUShort((uint16_t*)var); var_value = *(uint16_t*)var; break;
        case 4: __CrestUInt((uint32_t*)var); var_value = *(uint32_t*)var; break;
        case 8: return;
    }
//    printf("%s %d\n", name, var_value);
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
