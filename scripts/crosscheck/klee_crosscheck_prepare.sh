#!/bin/bash
#Prepapres a version where two version get crosscheckes
#USAGE: ./crosscheck_prepare original ver2 output
#ver1 and ver2 need to be EMI, with_overllappign namespace
DIR_NAME=$(dirname "$(realpath $0)")
source $DIR_NAME/../settings.sh
MAIN_REMOVE="$GH_PROJECT_ROOT/src2src/build/tool -extra-arg="-I$INST_LIB_PATH" -extra-arg="-I$CSMITH_RUNTIME" -r"
VERT_1=$(mktemp)
VERT_2=$(mktemp)
PREFIXED=$(mktemp --suffix=.c)
ORIG_NAME=$(basename $1)
NATIVE_CAR=$GH_PROJECT_ROOT/scripts/CaR/nativeCompileAndRun.sh 
NULL=/dev/null

$MAIN_REMOVE $1 2>$NULL  | tail -n+3 > $VERT_1
$DIR_NAME/prefix.sh 'pref_' $2 > $PREFIXED 
$MAIN_REMOVE $PREFIXED 2>$NULL  > $VERT_2

LIB_CHOICE=klee/crosscheck_helper/
$NATIVE_CAR $1 $ORIG_NAME | grep -v "checksum = " | cat $VERT_1 $VERT_2 - > $3
rm $VERT_1 $VERT_2 $PREFIXED
