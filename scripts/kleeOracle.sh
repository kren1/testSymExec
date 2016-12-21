#!/bin/bash
#Runs the native and  klee version and compares them using output and funcalls oracle
#USAGE: ./kleeOracle.sh ./native.o klee.bc 
NATIVE_EXEC=$1
KLEE_EXEC=$2
KLEE_OUT=$(mktemp)
NATIVE_OUT=$(mktemp)
ERROR_PIPE=$(mktemp)
DIR_NAME=$(dirname "$(realpath $0)")
FUNC_CALLS_OUT_NAME=$(basename $1 .o).c.out
NATIVE_FUNC_OUT_NAME=$(mktemp)
source $DIR_NAME/settings.sh

rm $FUNC_CALLS_OUT_NAME
timeout 1 $NATIVE_EXEC > $NATIVE_OUT 2> $ERROR_PIPE &&\
mv $FUNC_CALLS_OUT_NAME $NATIVE_FUNC_OUT_NAME &&\
timeout 100 $KLEE $KLEE_EXEC > $KLEE_OUT 2> $ERROR_PIPE  &&\
diff $NATIVE_OUT $KLEE_OUT &&\
STATUS="Output ok" &&\
diff $FUNC_CALLS_OUT_NAME $NATIVE_FUNC_OUT_NAME &&\
STATUS="$STATUS, Funcalls ok" &&\
rm -r $NATIVE_OUT $KLEE_OUT $ERROR_PIPE  $FUNC_CALLS_OUT_NAME $NATIVE_FUNC_OUT_NAME &&\
exit 0 ||\
echo $STATUS $NATIVE_OUT $KLEE_OUT $ERROR_PIPE $FUNC_CALLS_OUT_NAME $NATIVE_FUNC_OUT_NAME 
exit 1

