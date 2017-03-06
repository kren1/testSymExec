/*
    Implemntation of symbolizing interface that does nothing, suitable for making native runs
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>


void symbolize_and_constrain_s(uint8_t *var, int size, int64_t value, char* name) {}

void symbolize_and_constrain_u(void *var, int size, uint64_t value, char* name) {}

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
