#!/bin/bash
#USAGE ./kleeCompileAndRun test1.c test1.c
DIR_NAME=$(dirname "$(realpath $0)")
source $DIR_NAME/../settings.sh
set -x

BC_FILE=$(mktemp)
LINKED_BC_FILE=$(mktemp)
KLEE_OUT_DIR=$(mktemp -d)
$CLANG -xc -I$INST_LIB_PATH -I$CSMITH_RUNTIME -o $BC_FILE -c -emit-llvm $1 2> /dev/null &&\
$LINK -o=$LINKED_BC_FILE $BC_FILE $INST_LIB_PATH/$LIB_CHOICE/*.bc &&\
START=$(date +%s.%N) &&\
rmdir $KLEE_OUT_DIR &&\ #need to remove so klee can recreate 
timeout 100 klee -output-dir=$KLEE_OUT_DIR $LINKED_BC_FILE
EXIT_STATUS=$?
DURATION=$(echo "$(date +%s.%N) - $START" | bc) &&\

#Prints the function call trace to stdout and deletes it for cleanup. 
cat $(basename $2).out &&\
rm $(basename $2).out

rm $BC_FILE $LINKED_BC_FILE
rm -r $KLEE_OUT_DIR
if [ $EXIT_STATUS == "124" ];
then
    echo "timeout" >> $2.info
    exit 0;
fi

if [ $EXIT_STATUS == "0" ];
then
    echo "k:${DURATION}" >> $2.info
    exit 0;
fi
exit 1
