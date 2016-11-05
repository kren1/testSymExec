#!/bin/bash
#USAGE: ./doTest 0

NULL=/dev/null

PREFIX='test'${1:-'0'}
C_NAME=${PREFIX}.c
NATIVE_NAME=${PREFIX}.o
KLEE_NAME=${PREFIX}.bc

DIR_NAME=$(dirname "$(realpath $0)")
echo "Generating $C_NAME" &&\
${DIR_NAME}/icsmith.sh $C_NAME &&\
echo "Compiling native $NATIVE_NAME" &&\
${DIR_NAME}/nativeCompile.sh $C_NAME $NATIVE_NAME 2> $NULL &&\
echo "Compiling klee $KLEE_NAME" &&\
${DIR_NAME}/kleeCompile.sh $C_NAME $KLEE_NAME 2> $NULL  &&\
echo "Success!"
