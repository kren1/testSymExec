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
NAME='test70.c'
#Check that the given file doesn't have any weird behaviour by compiling it with sctrict policy and running it natively
CLANG=~/dependencies/llvm-3.9/bin/clang
$CLANG -fsanitize=address -fsanitize=bounds -fsanitize=undefined -Werror=extra -w \
        -Werror=all -Wno-error=unused-value -Wno-error=unused-variable -Wno-error=parentheses-equality \
        -Wno-error=implicit-function-declaration -Wno-error=self-assign -Wno-error=unused-function \
        -Wno-error=unused-parameter -Wno-error=sign-compare -Wno-error=ignored-qualifiers -Werror=int-conversion\
        -o $NATIVE_O_FILE $INST_LIB_PATH/native/*.c  -I$CSMITH_RUNTIME $NAME &&\
timeout 5 $NATIVE_O_FILE > $NATIVE_OUT &&\
grep checksum $NATIVE_OUT > $DIFF_FILE &&\
! grep '...checksum after hashing' $NATIVE_OUT > $DIFF_FILE &&\
$DIR_NAME/generalSingleExperiment.sh $NAME 2> $ERRS >&2 &&\
echo "remove" > /dev/null
#! $DIR_NAME/CaR/fuzzballCompileAndRun.sh $NAME `realpath $NAME` > $ERRS 2>&1 &&\
grep "> checksum" $ERRS &&\
! grep "KLEE: WARNING: unable to write output test case, losing it" $ERRS &&\
grep "> g_750: 127" $ERRS &&\
#grep "checksum" $ERRS | cut -f 2- -d ' '  | diff $DIFF_FILE - | wc -l | grep 2
grep "> g_297" $ERRS 
EXIT_CODE=$?
rm $NATIVE_O_FILE $NATIVE_OUT $ERRS $DIFF_FILE test70.c.out.lock
exit $EXIT_CODE
