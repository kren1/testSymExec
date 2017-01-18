#!/bin/bash
#example USAGE ./creduceTest.sh
DIR_NAME=$(dirname "$(realpath $0)")
source $DIR_NAME/settings.sh
source $DIR_NAME/setupCrestConstrainers.sh
NULL=/dev/null
NULL=/dev/tty
NATIVE_O_FILE=$(mktemp)
NATIVE_OUT=$(mktemp)
ERRS=$(mktemp)
NAME='test41.c'
#Check that the given file doesn't have any weird behaviour by compiling it with sctrict policy and running it natively
#set -x
$CLANG -fsanitize=address -fsanitize=undefined -Werror=extra -w \
        -Werror=all -Wno-error=unused-value -Wno-error=unused-variable -Wno-error=parentheses-equality \
        -Wno-error=implicit-function-declaration -Wno-error=self-assign -Wno-error=unused-function \
        -Wno-error=unused-parameter -Wno-error=sign-compare -Wno-error=ignored-qualifiers \
        -o $NATIVE_O_FILE $INST_LIB_PATH/native/*.c  -I$CSMITH_RUNTIME $NAME &&\
timeout 5 $NATIVE_O_FILE > $NATIVE_OUT &&\
grep checksum $NATIVE_OUT > /dev/null &&\
$DIR_NAME/generalSingleExperiment.sh $NAME 2> $ERRS >&1 &&\
#$DIR_NAME/CaR/crestCompileAndRun.sh $NAME `realpath $NAME` > $NATIVE_OUT 2>&1 &&\
grep "> func_21" $ERRS &&\
grep "< func_21" $ERRS 
EXIT_CODE=$?
rm $NATIVE_O_FILE $NATIVE_OUT $ERRS
exit $EXIT_CODE
