#!/bin/bash
#USAGE ./nativeCompileAndRun /path/to/source.c funCallsOutName.c
DIR_NAME=$(dirname "$(realpath $0)")
source $DIR_NAME/../settings.sh
NULL=/dev/null
NULL=/dev/tty

rm $(basename $2).out 2> $NULL
O_FILE=$(mktemp)
LINKED_FILE=$(mktemp)
echo '#include "instrument_lib.h"' | cat - $1 | $CLANG -xc -c -I$INST_LIB_PATH -I$CSMITH_RUNTIME -o $O_FILE - 2> $NULL &&\
$CLANG -o $LINKED_FILE $O_FILE $INST_LIB_PATH/native/build/*.o 2> $NULL &&\
START=$(date +%s.%N) &&\
timeout 1 $LINKED_FILE
DURATION=$(echo "$(date +%s.%N) - $START" | bc) &&\
EXIT_STATUS=$?
#Prints the function call trace to stdout and deletes it for cleanup. 
cat $(basename $2).out &&\
rm $(basename $2).out


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
