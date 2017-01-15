#!/bin/bash
#This script generates a test case, verifies that it terminates and symbolizes it
DIR_NAME=$(dirname "$(realpath $0)")
source $DIR_NAME/../settings.sh
INSTRUMENT_TOOL=$GH_PROJECT_ROOT/src2src/build/tool
TMP_FILE=$(mktemp)

while
    csmith -o $1 --no-divs --no-muls --no-math64 --no-return-structs --no-return-unions
    gcc -I$CSMITH_RUNTIME -o $TMP_FILE $1
    timeout  2 $TMP_FILE
    [ $? -ne 0 ]; 
do
    continue
done


$INSTRUMENT_TOOL -extra-arg="-I$CSMITH_RUNTIME"  $1 --  > $TMP_FILE 
mv $TMP_FILE $1
chmod a-x $1
