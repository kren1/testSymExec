#!/bin/bash
#USAGE ./nativeCompileAndRun /path/to/source.c funCallsOutName.c
DIR_NAME=$(dirname "$(realpath $0)")
source $DIR_NAME/../../settings.sh

rm $(basename $2).out
O_FILE=$(mktemp)
LINKED_FILE=$(mktemp)
gcc -fprofile-arcs -ftest-coverage -O0 -xc -g -c -I$INST_LIB_PATH -I$CSMITH_RUNTIME -o $O_FILE $1 > /dev/null &&\
gcc -fprofile-arcs -ftest-coverage -o $LINKED_FILE $O_FILE $INST_LIB_PATH/native/build/*.o > /dev/null &&\
START=$(date +%s.%N) &&\
timeout 1 $LINKED_FILE
DURATION=$(echo "$(date +%s.%N) - $START" | bc) &&\
EXIT_STATUS=$?
#Prints the function call trace to stdout and deletes it for cleanup. 
cat $(basename $2).out &&\
rm $(basename $2).out
GCOV_PREFIX=$(echo $O_FILE | tr . o)
mv $O_FILE.gcda $GCOV_PREFIX.gcda
mv $O_FILE.gcno $GCOV_PREFIX.gcno
gcov -o $GCOV_PREFIX $1 > /dev/null &&\
grep -A10000 ' This is a RANDOMLY GENERATED PROGRAM.' $1.gcov | sed -e 's/:[ \t0-9]*:/:/'


rm $O_FILE $LINKED_FILE $GCOV_PREFIX.gcda $GCOV_PREFIX.gcno $1.gcov
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
