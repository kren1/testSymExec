#!/bin/bash
#example USAGE ./creduceTest.sh
DIR_NAME=$(dirname "$(realpath $0)")
source $DIR_NAME/settings.sh
NULL=/dev/null
NULL=/dev/tty
NATIVE_O_FILE=$(mktemp)
NATIVE_OUT=$(mktemp)
ERRS=$(mktemp)
DIFF_FILE=$(mktemp)
NAME='test442.c'
#Check that the given file doesn't have any weird behaviour by compiling it with sctrict policy and running it natively
CLANG=~/dependencies/llvm-3.9/bin/clang
$CLANG -fsanitize=address -fsanitize=bounds -fsanitize=undefined -Werror=extra -w \
        -Werror=all -Wno-error=unused-value -Wno-error=unused-variable -Wno-error=parentheses-equality \
        -Wno-error=implicit-function-declaration -Wno-error=self-assign -Wno-error=unused-function \
        -Wno-error=unused-parameter -Wno-error=sign-compare -Wno-error=ignored-qualifiers \
        -o $NATIVE_O_FILE $INST_LIB_PATH/native/*.c  -I$CSMITH_RUNTIME $NAME &&\
timeout 5 $NATIVE_O_FILE > $NATIVE_OUT &&\
grep checksum $NATIVE_OUT > $DIFF_FILE &&\
$DIR_NAME/generalSingleExperiment.sh $NAME 2> $ERRS >&1 &&\
#! $DIR_NAME/CaR/fuzzballCompileAndRun.sh $NAME `realpath $NAME` > $ERRS 2>&1 &&\
grep "> checksum" $ERRS &&\
grep "< checksum" $ERRS &&\
grep "checksum" $ERRS | cut -f 2- -d ' '  | diff $DIFF_FILE - | wc -l | grep 2
#grep "< g_18" $ERRS &&\
#grep "> g_18" $ERRS &&\
#grep "> g_82" $ERRS &&\
#grep "< g_82" $ERRS &&\
EXIT_CODE=$?
rm $NATIVE_O_FILE $NATIVE_OUT $ERRS $DIFF_FILE
exit $EXIT_CODE
