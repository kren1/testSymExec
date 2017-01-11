#include <stdint.h>
int logFunction(char* name);

void symbolize_and_constrain_s(uint8_t *var, int size, int64_t value, char* name);

void symbolize_and_constrain_u(void *var, int size, uint64_t value, char* name);

void print_symbolic(const char* name, int64_t *val, char size);

