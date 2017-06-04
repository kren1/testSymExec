#!/bin/bash
#USAGE ./kleeCompileAndRun test1.c test1.c
DIR_NAME=$(dirname "$(realpath $0)")
source $DIR_NAME/../settings.sh


BC_FILE=$(mktemp)
LINKED_BC_FILE=$(mktemp)
#need to removei before klee is called so klee can recreate 
KLEE_OUT_DIR_TEMP=$(mktemp -d)
KLEE_OUT_DIR=${KLEE_DIR:-$KLEE_OUT_DIR_TEMP}
$CLANG -xc -I$INST_LIB_PATH -I$CSMITH_RUNTIME $OTHER_FLAGS -o $BC_FILE -c -emit-llvm $1 2> /dev/null &&\
$LINK -o=$LINKED_BC_FILE $BC_FILE $INST_LIB_PATH/$LIB_CHOICE/*.bc &&\
rm -r $KLEE_OUT_DIR 
START=$(date +%s.%N) &&\
timeout -s SIGKILL 520 timeout 500 klee-new -emit-all-errors -use-forked-solver=0 -max-time=590 -watchdog -search=dfs -libc=none -allow-external-sym-calls  -output-dir=$KLEE_OUT_DIR $LINKED_BC_FILE
EXIT_STATUS=$?
DURATION=$(echo "$(date +%s.%N) - $START" | bc) &&\

#Prints the function call trace to stdout and deletes it for cleanup. 
cat $(basename $2).out &&\
rm $(basename $2).out
rm $(basename $2).out.lock /tmp/$(basename $2).out.lock 2> /dev/null

klee-stats $KLEE_OUT_DIR >&2

rm $BC_FILE $LINKED_BC_FILE
rm -r $KLEE_OUT_DIR_TEMP
if [ $EXIT_STATUS == "124" -o $EXIT_STATUS == "137" ];
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
