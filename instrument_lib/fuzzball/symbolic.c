/*
    Implemntation of symbolizing interface that does very little
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include "constrainers/lt.h"

#define SYM_SIZE 4

 uint8_t  symbolic8() { return 8; };
 uint16_t symbolic16(){ return 16; };
 uint32_t symbolic32(){ return 32; };
// uint64_t symbolic64(){ return 64; };


static uint64_t magic_symbols[SYM_SIZE] = {0,0,0,3};
static uint8_t *symarray = (uint8_t*)magic_symbols;
static int sym_pos = 0;

void symbolize_and_constrain_s(uint8_t *var, int size, int64_t value, char* name) {}

void symbolize_and_constrain_u(uint32_t *var, int size, uint32_t value, char* name) {
//    printf("%s, %d\n", name, size);
    if(size == 8) return;
    if(sym_pos + size > SYM_SIZE * 8) return;
//    printf("%s, %d,  sym pos - %d\n", name, size, sym_pos);

    uint32_t v;
    switch(size) 
    {
        case 1: v = *(uint8_t*)(symarray + sym_pos); *(uint8_t*)var = v; break;
        case 2: v = *(uint16_t*)(symarray + sym_pos); *(uint16_t*)var = v; break;
        case 4: v = *(uint32_t*)(symarray + sym_pos); *(uint32_t*)var = v; break;
    }

    sym_pos =  sym_pos + size;
    CONSTRAIN(v, value, exit(0));
}

void print_symbolic(const char* name, uint64_t *val, char size)
{
//    if(size != 32) return;
//    printf("printing ... ");
    int64_t v; 
    switch(size)
    {
        case 8: v = *(int8_t*)val; break;
        case 16: v = *(int16_t*)val; break;
        case 32: v = *(int32_t*)val; break;
        case 64: v = *(int64_t*)val; break;
    }
    printf("%s: %lld\n",name,v);

}
