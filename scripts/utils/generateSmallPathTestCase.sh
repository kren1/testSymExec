#!/bin/bash
#This script generates test case with small number of paths so that klee can terminate completly
DIR_NAME=$(dirname "$(realpath $0)")
source $DIR_NAME/../settings.sh
INSTRUMENT_TOOL=$GH_PROJECT_ROOT/src2src/build/tool
source $DIR_NAME/../setups/kleeTerminatingModeC.sh
TMP_FILE=$(mktemp --suffix='.c')
#TMP_FILE=$1
#set -x

REGEX='(?<=KLEE: done: completed paths = )([0-9]+)'

TIMEOUT_CNT=0

while
    $DIR_NAME/generateTestCase.sh $TMP_FILE 2> /dev/null
    KLEE_OUT=$(cd $(dirname $TMP_FILE) && $COMPILE_AND_RUN_2 $TMP_FILE 2>&1) 
    PATH_CNT=$(echo $KLEE_OUT | grep -Po "$REGEX")
    echo $KLEE_OUT | grep "KLEE: halting execution" > /dev/null
    #halted = 0, means the klee exeuction had a timeout
    HALTED=$?
    rm  ${TMP_FILE}.out
    [ ${PATH_CNT:-1} -lt 30 -o $HALTED -eq 0 ];
do
    if [  ]; then
        TIMEOUT_CNT=$((TIMEOUT_CNT + 1))
    fi
    continue
done

echo "$1 timeout count: $TIMEOUT_CNT" >> timeoutcnt_log

$DIR_NAME/renameTestCase.sh $TMP_FILE $1
