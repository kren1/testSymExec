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
#include <assert.h>
#ifdef READ_PATH
#include <errno.h>
#endif

#define NO_MORE_READ 49

void klee_silent_exit(int x){}


void symbolize_and_constrain_s(uint8_t *var, int size, int64_t value, char* name) {}

FILE* path_file = NULL;
void symbolize_and_constrain_u(void *var, int size, uint64_t value, char* name) {
#ifdef READ_PATH
    uint8_t buf[size];
    uint8_t offset = 0;
    if(path_file == NULL) {
        path_file = fopen(getenv("BIN_FILE"),"rb");

        assert(path_file != NULL);
        pid_t pid = fork();
        while(pid != 0) {
           int status;
           //patrent code
           waitpid(pid, &status, 0);
//           fprintf(stderr,"parent file pos %ld, %d\n", ftell(path_file), errno);
           if(WEXITSTATUS(status) == NO_MORE_READ) {
                exit(0);
           }
           pid = fork();
        }
        //child continues as is 
    }
//    fprintf(stderr,"child file pos %ld, size %d\n", ftell(path_file), size);
    size_t read_bytes = fread(buf ,size, 1, path_file);
    if(read_bytes == 0) exit(NO_MORE_READ);
    uint64_t val;
    switch(size) 
    {
        case 1: val = *buf; break;
        case 2: val = *(uint16_t*)buf; break;
        case 4: val = *(uint32_t*)buf; break;
        case 8: val = *(uint64_t*)buf; break;
    }
//    fprintf(stderr, "%s(%d): %"PRId64"\n",name, size, (int64_t)val);
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
