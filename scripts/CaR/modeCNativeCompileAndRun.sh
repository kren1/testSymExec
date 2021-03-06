#!/bin/bash
#USAGE ./nativeCompileAndRun /path/to/source.c funCallsOutName.c
DIR_NAME=$(dirname "$(realpath $0)")
source $DIR_NAME/../settings.sh

#rm $(basename $2).out
O_FILE=$(mktemp)
LINKED_FILE=$(mktemp)
$CLANG -xc -c -I$INST_LIB_PATH -I$CSMITH_RUNTIME -o $O_FILE $1 2> /dev/null &&\
$CLANG -o $LINKED_FILE $O_FILE $INST_LIB_PATH/native/modeC/build/*.o 2> /dev/null &&\
START=$(date +%s.%N) &&\
timeout 1 $LINKED_FILE
DURATION=$(echo "$(date +%s.%N) - $START" | bc) &&\
EXIT_STATUS=$?
#Prints the function call trace to stdout and deletes it for cleanup. 
#cat $(basename $2).out &&\
rm $(basename $2 2> /dev/null).out 2> /dev/null


rm $O_FILE $LINKED_FILE
if [ $EXIT_STATUS == "124" ];
then
    echo "timeout"
    exit 0;
fi

if [ $EXIT_STATUS == "0" ];
then
    echo "n:${DURATION}" >> $2.info
    exit 0;
fi

exit 1
