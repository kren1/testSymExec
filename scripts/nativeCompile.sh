#!/bin/bash

#USAGE ./nativeCompile test0.c test0.o


TEMP_FILE=linked_temp${1}.c
DIR_NAME=$(dirname "$(realpath $0)")
source $DIR_NAME/settings.sh

LINK_WITH="$INST_LIB_PATH/klee/symbolic.c $INST_LIB_PATH/klee/funCalls.c"

cat $LINK_WITH $1 > $TEMP_FILE &&\
$CLANG -I$CSMITH_RUNTIME -o $2 $TEMP_FILE
rm $TEMP_FILE

