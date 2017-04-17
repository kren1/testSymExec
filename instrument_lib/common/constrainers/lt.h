//Constrains symbolic value (sym_val) to real_val exiting with exit_stmt
#define CONSTRAIN(sym_val,  real_val, exit_stmt) \
if( sym_val < real_val) {\
    exit_stmt; \
} \
\
if( sym_val > real_val) {\
    exit_stmt; \
}
