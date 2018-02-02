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
char pref[10];


void symbolize_and_constrain_u(void *var, int size, uint32_t value, char* name) {
    call_cnt++;

    if(call_cnt == 1) {
        c_file = fopen(__klee__instr_filename, "w");
        fprintf(c_file, "#include <assert.h>\n");
        fprintf(c_file, "int main() {\n");
        if(strstr(getenv("INSTRUMENTER"), "igress"))
          fprintf(c_file, "\t megaInit();\n");
        snprintf(pref,10, "%s", "pref_");
    }
    char pref_name[40];
    snprintf(pref_name, 40, "%s%s", pref, name);
    
    fprintf(c_file, "\tsymbolize_and_constrain_u(&%s,sizeof %s,50, \"%s\");\n", name, name, name);
    fprintf(c_file, "\tsymbolize_and_constrain_u(&%s,sizeof %s,50, \"%s\");\n", pref_name, pref_name, pref_name);
    fprintf(c_file, "\tif(%s != %s) klee_silent_exit(0);\n\n", name, pref_name);



}
char first = 1;
void print_symbolic(const char* name, int64_t *val, char size)
{
    call_cnt--;
    if(first) {
        first = 0;
        fprintf(c_file, "\tfunc_1();\n");
        fprintf(c_file, "\t%sfunc_1();\n", pref);
    }

    //fprintf(c_file, "\tassert(%s%s == %s);\n", pref, name, name);
    fprintf(c_file, "\tif(%s%s != %s) printf(\"ASSERTION FAIL: %s\\n\");\n", pref, name, name,name);

    if(call_cnt == 0) {
        fprintf(c_file,"}\n");
        fclose(c_file);
    }
}
