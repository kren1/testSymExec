/*
To be linked with the file produced when usint Intrumentation.so -funcalls or -symglob for native runs
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
static uint32_t symbolizeCallCnt = 0;
void klee_silent_exit(int i)
{
}

void klee_make_symbolic(void* x, int y, char* name)
{
    return;
}
void symbolize_and_constrain_s(uint8_t *var, int size, int64_t value, char* name) {
}

void symbolize_and_constrain_u(void *var, int size, uint64_t value, char* name) {
    if(symbolizeCallCnt > 8) return;
    symbolizeCallCnt++;
    klee_make_symbolic(var, size, name);
 
}
void print_symbolic(const char* name, int64_t *val, char size)
{
     return;
    switch(size)
    {
        case 8: printf("%s: %hhd\n",name,*(int8_t*)val); break;
        case 16: printf("%s: %hd\n",name,*(int16_t*)val); break;
        case 32: printf("%s: %d\n",name,*(int32_t*)val); break;
        case 64: printf("%s: %" PRId64 "\n",name,*val); break;
    }
}
