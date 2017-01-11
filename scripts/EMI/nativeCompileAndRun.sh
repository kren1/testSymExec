#!/bin/bash
#USAGE ./nativeCompileAndRun /path/to/source.c funCallsOutName.c.out
DIR_NAME=$(dirname "$(realpath $0)")
source $DIR_NAME/../settings.sh

O_FILE=$(mktemp)
LINKED_FILE=$(mktemp)
clang-3.4 -xc -c -I$INST_LIB_PATH -I$CSMITH_RUNTIME -o $O_FILE $1 > /dev/null &&\
clang-3.4 -o $LINKED_FILE $O_FILE $INST_LIB_PATH/native/build/*.o > /dev/null &&\
timeout 1 $LINKED_FILE
EXIT_STATUS=$?
#Prints the function call trace to stdout and deletes it for cleanup. 
cat $2 &&\
rm $2

rm $O_FILE $LINKED_FILE
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
