#!/bin/bash
#USAGE ./kleeCompileAndRun test1.c test1.c
DIR_NAME=$(dirname "$(realpath $0)")
source $DIR_NAME/../settings.sh

BC_FILE=$1.bc #$(mktemp)
LINKED_BC_FILE=linked.bc # $(mktemp)
OPT_BC_FILE=opted.bc
LB=/home/symext/dependencies/llvm-3.4/bin/
KLEE_BUILD=/home/symext/dependencies/modeCklee/klee/build/Release
$CLANG -xc -I$INST_LIB_PATH -I$CSMITH_RUNTIME -o $BC_FILE -c -emit-llvm -w $1 > /dev/null &&\
$LB/opt --load $KLEE_BUILD/lib/Instrumentation.so -insert-klee-coverage -o $OPT_BC_FILE $BC_FILE &&\
$LINK -o=$LINKED_BC_FILE $OPT_BC_FILE $INST_LIB_PATH/$LIB_CHOICE/*.bc &&\
START=$(date +%s.%N) &&\
timeout 100 klee $LINKED_BC_FILE
EXIT_STATUS=$?
DURATION=$(echo "$(date +%s.%N) - $START" | bc) &&\

#Prints the function call trace to stdout and deletes it for cleanup. 
cat $(basename $2).out &&\
rm $(basename $2).out

#rm $BC_FILE $LINKED_BC_FILE
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
