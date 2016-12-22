#!/bin/bash
#Runs the native and crest version and compares them using output and funcalls oracle
#Should run in the folder crest used for compilation!!!!!
#USAGE: ./crestOracle.sh ./native.o klee.bc 
NATIVE_EXEC=$1
NATIVE_OUT=native.out
ERROR_PIPE=run.err
CREST_OUT=crest.out
DIR_NAME=$(dirname "$(realpath $0)")
FUNC_CALLS_OUT_NAME=$(basename $1 .o).c.out
NATIVE_FUNC_OUT_NAME=nativeFunCalls.out
source $DIR_NAME/settings.sh
CREST_EXEC=$CREST_ROOT/bin/run_crest

timeout 1 $NATIVE_EXEC > $NATIVE_OUT 2> $ERROR_PIPE &&\
mv $FUNC_CALLS_OUT_NAME $NATIVE_FUNC_OUT_NAME &&\
timeout 100 $CREST_EXEC ./$2 1000 -dfs > $CREST_OUT 2> $ERROR_PIPE  &&\
diff $NATIVE_OUT $CREST_OUT &&\
STATUS="Output ok" &&\
diff $FUNC_CALLS_OUT_NAME $NATIVE_FUNC_OUT_NAME &&\
STATUS="$STATUS, Funcalls ok" &&\
exit 0 ||\
exit 1
