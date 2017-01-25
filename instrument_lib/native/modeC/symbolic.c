/*
    Implemntation of symbolizing interface that does nothing, suitable for making native runs
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <inttypes.h>

FILE* varFile = NULL;
//IF this flag is true, the var file is empty and variables should retain their values
bool emptyVarFile = false;

void symbolize_and_constrain_s(uint8_t *var, int size, int64_t value, char* name) {}

void symbolize_and_constrain_u(void *var, int size, uint64_t value, char* name) {
//    printf("env: %s;", getenv("VARS_FILENAME"));
    printf("%s: %u \t",name, value);
    if(varFile == NULL) {
        printf("Opening File %s\n", getenv("VARS_FILENAME"));
        if(! (varFile = fopen(getenv("VARS_FILENAME"), "r"))) {
            printf("Cannot open vars file %s\n exiting .. \n", getenv("VARS_FILENAME"));
            exit(0);
        } 
        //check if file is empty
        fseek(varFile, 0, SEEK_END);
        if(ftell(varFile) == 0) {
            emptyVarFile = true;
        }
        rewind(varFile);
    }

    if(value >  1294967295 || emptyVarFile) {
        printf(" skip\n");
        return;
    }

    uint32_t readVal;
    fscanf(varFile, "%u", &readVal);
    printf("%s: %u \n", name,readVal);

    switch(size)
    {
        case 1:  *(uint8_t*)var = readVal; break;
        case 2:  *(uint16_t*)var = readVal; break;
        case 4:  *(uint32_t*)var = readVal; break;
        case 8: return;
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
