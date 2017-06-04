#!/bin/bash
#This script reinstruments the test case
#usage: renameTestCase path/to/testCase 
DIR_NAME=$(dirname "$(realpath $0)")
source $DIR_NAME/../settings.sh
TMP=$(mktemp --suffix=.c)
INSTRUMENT_TOOL=$GH_PROJECT_ROOT/src2src/build/tool
NULL=/dev/null

grep -v '__klee__instr_filename' $1 | grep -v 'logFunction' | grep -v 'symbolize_and_constrain' | grep -v 'print_symbolic' > $TMP
#rm $1
cp $TMP $1
$INSTRUMENT_TOOL -extra-arg="-I$CSMITH_RUNTIME"  $1 --  > $TMP 2> $NULL
cp $TMP $1
rm $TMP

