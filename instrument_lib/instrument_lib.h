#include <stdint.h>
int logFunction(char* name);

void symbolize_and_constrain_s(uint8_t *var, int size, int64_t value, char* name);

void symbolize_and_constrain_u(uint32_t *var, int size, uint32_t value, char* name);

void print_symbolic(const char* name, int64_t *val, char size);

