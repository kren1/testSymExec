#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

extern char* __klee__instr_filename;

int logFunction(char* name) { return 0;}
void klee_silent_exit(int i){}
void klee_make_symbolic(void* x, int y, char* name){}
void symbolize_and_constrain_s(uint8_t *var, int size, int64_t value, char* name) {}

uint32_t call_cnt = 0;
FILE* c_file;


void symbolize_and_constrain_u(void *var, int size, uint64_t value, char* name) {
    call_cnt++;

    if(call_cnt == 1) {
        c_file = fopen(__klee__instr_filename, "w");
        fprintf(c_file, "int main() {\n");
    }
    char pref_name[40];
    snprintf(pref_name, 40, "%s%s", "pref_", name);
    
    fprintf(c_file, "\tklee_make_symbolic(&%s,sizeof %s, \"%s\");\n", name, name, name);
    fprintf(c_file, "\tklee_make_symbolic(&%s,sizeof %s, \"%s\");\n", pref_name, pref_name, pref_name);
    fprintf(c_file, "\tklee_assume(%s == %s);\n\n", name, pref_name);


}
void print_symbolic(const char* name, int64_t *val, char size)
{
    call_cnt--;
    if(call_cnt == 0) {
        fclose(c_file);
    }
}
