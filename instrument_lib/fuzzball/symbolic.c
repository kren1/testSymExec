/*
    Implemntation of symbolizing interface that does very little
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>

#define SYM_SIZE 4

 uint8_t  symbolic8() { return 8; };
 uint16_t symbolic16(){ return 16; };
 uint32_t symbolic32(){ return 32; };
// uint64_t symbolic64(){ return 64; };


static uint32_t magic_symbols[SYM_SIZE] = {0,0,0,3};
static int sym_pos = 0;

void symbolize_and_constrain_s(uint8_t *var, int size, int64_t value, char* name) {}

void symbolize_and_constrain_u(uint32_t *var, int size, uint32_t value, char* name) {
    printf("%s, %d\n", name, size);
    if(size == 1) {
        printf("fsdf 1\n");
        return;
    }
    if(size != 4 && name == NULL) return;
    if(sym_pos + size > SYM_SIZE) return;
    printf("%s, %d\n", name, size);

    uint32_t v = *(magic_symbols + sym_pos);
    sym_pos += size / 4;
    
    *var = v;

    if( v < value) {
        printf("branch 1 \n");
        exit(0);
    }
  
    if(v > value ) {
        printf("branch 2 \n");
        exit(0);
    }

}

void print_symbolic(const char* name, int64_t *val, char size)
{
    if(size != 32) return;
    switch(size)
    {
        case 8: printf("%s: %d\n",name,*(int8_t*)val); break;
        case 16: printf("%s: %d\n",name,*(int16_t*)val); break;
        case 32: printf("%s: %d\n",name,*(int32_t*)val); break;
        case 64: printf("%s: %ld\n",name,*val); break;
    }
}
