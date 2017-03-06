/*
To be linked with the file produced when usint Intrumentation.so -funcalls or -symglob for native runs
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern char* __klee__instr_filename;
int logFunction(char* name) 
{
     if(strlen(name) == 0) return 0;
     FILE* f = fopen(__klee__instr_filename, "a");
     if (f == NULL)
     {
          return 1;
     }
    fprintf(f,"%s\n",name);
    fclose(f);
    return 0;
}
