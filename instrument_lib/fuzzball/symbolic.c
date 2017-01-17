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


static char magic_symbols[SYM_SIZE] = {0,0,0,3};
static int sym_pos = 0;

void symbolize_and_constrain_s(uint8_t *var, int size, int64_t value, char* name) {}

void symbolize_and_constrain_u(uint8_t *var, int size, uint32_t value, char* name) {
    printf("%s, %d\n", name, size);
    if(size != 4 && name == NULL) return;
    if(sym_pos + size > SYM_SIZE) return;

    uint32_t v = *(uint32_t*)(magic_symbols + sym_pos);
    sym_pos += size;
    
    
//    *var = sym_var;
    
/*
    switch(size)
    {
         case 1: *(uint8_t*)var = symbolic8() ; var_value = *(uint8_t*)var; break;
         case 2: *(uint16_t*)var = symbolic16() ; var_value = *(uint16_t*)var; break;
         case 4: *(uint32_t*)var = symbolic32() ; var_value = *(uint32_t*)var; break;
         case 8: return;//*(uint64_t*)var = symbolic64() ; var_value = *(uint64_t*)var; break;
    }
*/
    printf("%s: %d vs %d\n",name, *var, value);
    if( v < value) {
        exit(0);
    }
  
    if(v > value ) {
        exit(0);
    }
    *var = v;

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
