#!/bin/bash
#USAGE: ./doCrestTest 0

NULL=/dev/null

PREFIX='test'${1:-'0'}
C_NAME=${PREFIX}.c
OBJECT_NAME=${PREFIX}.o
ORIGINAL_DIR=$(pwd)

DIR_NAME=$(dirname "$(realpath $0)")
echo "Generating $C_NAME" >&2 &&\
${DIR_NAME}/icsmith.sh $C_NAME &&\
echo "Compiling native $OBJECT_NAME" >&2 &&\
${DIR_NAME}/nativeCompile.sh $C_NAME $OBJECT_NAME 2> $NULL &&\
echo "Compiling crest $OBJECT_NAME" >&2 &&\
CREST_COMPILATION_DIR=$(${DIR_NAME}/crestCompile.sh $C_NAME $OBJECT_NAME)  &&\
ln -s $CREST_COMPILATION_DIR ${PREFIX}.crest.d &&\
cd $CREST_COMPILATION_DIR &&\
echo "Running oracle in $CREST_COMPILATION_DIR" >&2 &&\
${DIR_NAME}/crestOracle.sh $ORIGINAL_DIR/$OBJECT_NAME $OBJECT_NAME &&\
echo "Success!" >&2
