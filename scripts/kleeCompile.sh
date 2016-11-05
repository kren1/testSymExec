#!/bin/bash

#USAGE ./compileForKlee test0.c test0.bc

CLANG=clang-3.4
LINK=llvm-link-3.4
LIB_PATH=/home/tim/projects/testSymExec/instrument_lib

TEMP_FILE=comp_temp${1}.bc
LINK_WITH="$LIB_PATH/klee/build/symbolic.bc $LIB_PATH/klee/build/funCalls.bc"

$CLANG -c -emit-llvm -I$CSMITH_RUNTIME -o $TEMP_FILE $1
$LINK -o $2 $TEMP_FILE $LINK_WITH
rm $TEMP_FILE
exit 0
