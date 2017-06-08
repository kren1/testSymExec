/*
    Implemntation of symbolizing interface that does nothing, suitable for making native runs
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdio.h>


#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void klee_silent_exit(int x){}


void symbolize_and_constrain_s(uint8_t *var, int size, int64_t value, char* name) {}

FILE* path_file = NULL;
void symbolize_and_constrain_u(void *var, int size, uint64_t value, char* name) {
#ifdef READ_PATH
    if(path_file == NULL) {
        path_file = fopen("blarp.bin","rb");
        pid_t pid = fork();
        while(pid != 0) {
           //patrent code
           waitpid(pid, NULL, 0);
           printf("parent file pos %ld\n", ftell(path_file));
           if(feof(path_file)) {
            //replayed all the paths
            exit(0);
           }
           pid = fork();
        }
        //child continues as is 
    }
    printf("child file pos %ld\n", ftell(path_file));
    uint8_t buf[size];
    fread(buf ,size, 1, path_file);
    uint64_t val;
    switch(size) 
    {
        case 1: val = *buf; break;
        case 2: val = *(uint16_t*)buf; break;
        case 4: val = *(uint32_t*)buf; break;
        case 8: val = *(uint64_t*)buf; break;
    }
    fprintf(stderr, "%s: %"PRIu64"\n",name, val);
#endif
}

void print_symbolic(const char* name, int64_t *val, char size)
{
    switch(size)
    {
        case 8: printf("%s: %d\n",name,*(int8_t*)val); break;
        case 16: printf("%s: %d\n",name,*(int16_t*)val); break;
        case 32: printf("%s: %d\n",name,*(int32_t*)val); break;
        case 64: printf("%s: %" PRId64 "\n",name,*val); break;
    }
}
