/*
To be linked with the file produced when usint Intrumentation.so -funcalls or -symglob for native runs
*/
#include <stdio.h>
#include <stdlib.h>

extern char* __klee__instr_filename;
int logFunction(char* name) 
{
     if(strlen(name) == 0) return 0;
     char filename[100];
     snprintf(filename, 100,"iter_%s_%s", getenv("ITER_NUM"),__klee__instr_filename);


//     printf("env: %s \n",getenv("ITER_NUM")); 
     FILE* f = fopen(__klee__instr_filename, "a");
     //FILE* f = fopen(filename, "a");
     if (f == NULL)
     {
          return 1;
     }
    fprintf(f,"%s\n",name);
    fclose(f);
    return 0;
}
