#!/bin/bash

#USAGE ./compileForKlee test0.c test0.bc
set -x

CLANG=clang-3.4

TEMP_FILE=linked_temp${1}.c

LIB_PATH=/home/tim/projects/testSymExec/instrument_lib
LINK_WITH="$LIB_PATH/klee/symbolic.c $LIB_PATH/klee/funCalls.c"

cat $LINK_WITH $1 > $TEMP_FILE &&\
$CLANG -I$CSMITH_RUNTIME -o $2 $TEMP_FILE
#rm $TEMP_FILE
