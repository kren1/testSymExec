/*
To be linked with the file produced when usint Intrumentation.so -funcalls or -symglob for native runs
*/
#include <stdio.h>
#include <stdlib.h>

extern char* __klee__instr_filename;
static fnBuff[50];
char* get_file_name() 
{
   sprintf(fnBuff,"%s%d",__klee__instr_filename,klee_get_state_num());
    return fnBuff;
}
int logFunction(char* name) 
{
     FILE* f = fopen(get_file_name(), "a");
     if (f == NULL)
     {
          return 1;
     }
    fprintf(f,"%s\n",name);
    fclose(f);
//    printf("state %d\n",klee_get_state_num());
    return 0;
}

