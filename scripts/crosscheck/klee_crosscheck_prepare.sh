#!/bin/bash
#Prepapres a version where two version get crosscheckes
#USAGE: ./crosscheck_prepare original ver2 output
#ver1 and ver2 need to be EMI, with_overllappign namespace
DIR_NAME=$(dirname "$(realpath $0)")
source $DIR_NAME/../settings.sh
VERT_1=$(mktemp)
VERT_2=$(mktemp)
ORIG_NAME=$(basename $1)
NATIVE_CAR=$GH_PROJECT_ROOT/scripts/CaR/nativeCompileAndRun.sh 
cat $1 | sed -e '/int main/Q'  | tail -n+3 > $VERT_1
$DIR_NAME/prefix.sh 'pref_' $2 | sed -e '/int main/Q'   > $VERT_2

LIB_CHOICE=klee/crosscheck_helper/
$NATIVE_CAR $1 $ORIG_NAME | grep -v "checksum = " | cat $VERT_1 $VERT_2 - > $3
rm $VERT_1 $VERT_2
