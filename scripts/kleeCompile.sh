#!/bin/bash

#USAGE ./compileForKlee test0.c test0.bc

DIR_NAME=$(dirname "$(realpath $0)")
source $DIR_NAME/settings.sh


TEMP_FILE=comp_temp${1}.bc
LINK_WITH="$INST_LIB_PATH/klee/build/symbolic.bc $INST_LIB_PATH/klee/build/funCalls.bc"

$CLANG -O0 -c -emit-llvm -I$CSMITH_RUNTIME -o $TEMP_FILE $1
$LINK -o $2 $TEMP_FILE $LINK_WITH
rm $TEMP_FILE
exit 0
