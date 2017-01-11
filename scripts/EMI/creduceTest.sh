#!/bin/bash
#example USAGE ./creduceTest.sh test1.c
DIR_NAME=$(dirname "$(realpath $0)")
source $DIR_NAME/../settings.sh
NULL=/dev/null
NATIVE_O_FILE=$(mktemp)
NATIVE_OUT=$(mktemp)
ERRS=$(mktemp)
NAME='prep_test410.c'
#Check that the given file doesn't have any weird behaviour by compiling it with sctrict policy and running it natively
$CLANG -fsanitize=address -fsanitize=undefined -Werror=extra -w \
        -Werror=all -Wno-error=unused-value -Wno-error=unused-variable -Wno-error=parentheses-equality \
        -Wno-error=implicit-function-declaration -Wno-error=self-assign -Wno-error=unused-function \
        -Wno-error=unused-parameter -Wno-error=sign-compare -Wno-error=ignored-qualifiers \
        -o $NATIVE_O_FILE $INST_LIB_PATH/native/*.c  -I$CSMITH_RUNTIME $NAME &&\
timeout 5 $NATIVE_O_FILE > $NATIVE_OUT &&\
grep checksum $NATIVE_OUT > /dev/null &&\
! $DIR_NAME/emiTest.sh $NAME 2> $ERRS > $NULL &&\
grep 'Segmentation fault' $ERRS
