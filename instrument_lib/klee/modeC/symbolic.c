/*
To be linked with the file produced when usint Intrumentation.so -funcalls or -symglob for native runs
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <limits.h>
#include <errno.h>
#include <string.h>
static uint32_t symbolizeCallCnt = 0;
extern char* __klee__instr_filename;


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


#define FILE_BUFF_SIZE 100000
//#define DEBUG_DATA 0
int is_first_branch(char* locks_file, char* test_case_id) {
    FILE *fp = fopen(locks_file, "rb");
    #ifdef DEBUG_DATA
    fprintf(stderr,"locks file: %s, fp: %p\n",locks_file, fp);
    #endif

    if(fp != NULL) {
        char *file_buf = calloc(FILE_BUFF_SIZE,1);
        ssize_t rd = fread(file_buf, 1, FILE_BUFF_SIZE, fp);
        if(rd == 0) {
            fprintf(stderr,"Failed to read errno %d\n",errno);
            abort();
        }
        if(rd == FILE_BUFF_SIZE) {
            fprintf(stderr, "File %s, could not be read entierly, aborting...", locks_file);
            abort();
        }
        #ifdef DEBUG_DATA
        fprintf(stderr, "read: %d, data %s \n", rd, file_buf);
        #endif
        fclose(fp);
        //If we see our id in the file, we are not in the branch    
        //With the lowest number in range, under dfs assumption
        if(strstr(file_buf,test_case_id) != NULL) {
            free(file_buf);
            return 0;
        }
        free(file_buf);
    }
    return 1;
}

#define PADDING 10

//#define DEBUG 0
//#define DEBUG_DET 0
void print_symbolic(const char* name, int64_t *val, char size)
{
#ifdef LOWEST_SOLUTION
    int64_t ub, lb, lbForUb, ubForlb, prev, h;
    switch(size)
    {
        case 8: ub = SCHAR_MAX; lb = SCHAR_MIN; h = *(int8_t*)val; break;
        case 16: ub = SHRT_MAX; lb = SHRT_MIN; h = *(int16_t*)val; break;
        case 32: ub = INT_MAX; lb = INT_MIN; h = *(int32_t*)val;   break;
        default: printf("%s: %" PRId64 "\n",name,*val); //printf("Unimpleneted size %d, aboritng\n",size); abort();
    }
//    printf("after swtich %d\n",h);
    prev = ub + PADDING;
    lbForUb = lb;

    char test_case_id[20];
    char locks_file[20];
    snprintf(test_case_id, 20, "%d-", rand());
    snprintf(locks_file, 20, "%s.lock", __klee__instr_filename);

    #ifdef DEBUG
    fprintf(stderr,"for variable %s, test: %s\n", name, test_case_id);
    fprintf(stderr, "#0 lb: %ld, ub: %ld\n", lb, ub);
    #endif
    int iter_num = 0;
    while((prev - ub > 0) && is_first_branch(locks_file, test_case_id)){
        if(h < ub){
          prev = ub;
          ub = ub - (ub - lbForUb) / 2;
        } 
        //at this point ub is smaller than h can be, and prev is ub that can still shrink
        else {
            lbForUb = ub;
            ub = prev;
            prev = ub + 2;
        }
        iter_num++;
        #ifdef DEBUG_DET
        fprintf(stderr,"\t#U lbForUb: %ld, ub: %ld, prev: %ld \t\t\t\tin: %d\n", lbForUb, ub, prev, iter_num);
        #endif
    }
    #ifdef DEBUG
    fprintf(stderr, "#1 lb: %ld, ub: %ld\n", lb, ub);
    #endif
    //mirror case for lower bound
    ubForlb = ub;
    prev = lb - PADDING;
    while(lb - prev > 0 && is_first_branch(locks_file, test_case_id)) {
        if(!(h >= lb)) {
            ubForlb = lb;
            lb = prev; 
            prev = lb - 2;
        } else {
            prev = lb;
            lb = lb + (ubForlb - lb) / 2;
        }
        #ifdef DEBUG_DET
        fprintf(stderr,"\t#L lb: %ld, ubForlb: %ld, prev: %ld\n", lb, ubForlb, prev);
        #endif
    }

    //At this point we have narrowed down ub and lb to the lowest range
    if(!is_first_branch(locks_file, test_case_id)) {
        #ifdef DEBUG
        fprintf(stderr, "silent exit for lb: %ld, ub: %ld\n", lb, ub);
        #endif
        klee_silent_exit(0);
    }
    #ifdef DEBUG
    fprintf(stderr,"GOOD BRANCH  lb: %ld, ub: %ld\n", lb, ub);
    #endif

    FILE* fp = fopen(locks_file,"a");
    fprintf(fp, "%s", test_case_id);
    fclose(fp);
#endif

    switch(size)
    {
        case 8: printf("%s: %hhd\n",name,*(int8_t*)val); break;
        case 16: printf("%s: %hd\n",name,*(int16_t*)val); break;
        case 32: printf("%s: %d\n",name,*(int32_t*)val); break;
        case 64: printf("%s: %" PRId64 "\n",name,*val); break;
    }
}
