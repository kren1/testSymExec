#!/bin/bash
DIR_NAME=$(dirname "$(realpath $0)")
source $DIR_NAME/../settings.sh

BC_FILE=$(mktemp)
LINKED_BC_FILE=$(mktemp)
$CLANG -xc -I$INST_LIB_PATH -I$CSMITH_RUNTIME -o $BC_FILE -c -emit-llvm $1 > /dev/null &&\
$LINK -o=$LINKED_BC_FILE $BC_FILE $INST_LIB_PATH/klee/build/*.bc
timeout 100 klee $LINKED_BC_FILE
EXIT_STATUS=$?

#Prints the function call trace to stdout and deletes it for cleanup. 
cat $2 &&\
rm $2

rm $BC_FILE $LINKED_BC_FILE
if [ $EXIT_STATUS == "124" ];
then
    echo "timeout"
    exit 0;
fi

if [ $EXIT_STATUS == "0" ];
then
    exit 0;
fi

exit 1
