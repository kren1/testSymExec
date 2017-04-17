//Constrains symbolic value (sym_val) to real_val exiting with exit_stmt
#define CONSTRAIN(sym_val,  real_val, exit_stmt) \
if( sym_val <= real_val - 2 \
    || sym_val == real_val - 1 \
    || sym_val == real_val + 1 \
    || sym_val == real_val + 2 \
    || sym_val >= real_val + 3 ) { \
    exit_stmt; \
}

