#!/bin/bash
#instrumented csmith
#runs csmith and instruments the output
#USAGE: ./icsmith filename.c

CSMITH_OUT=$(mktemp)
DIR_NAME=$(dirname "$(realpath $0)")
#echo $CSMITH_OUT
mv $CSMITH_OUT ${CSMITH_OUT}.c
CSMITH_OUT=${CSMITH_OUT}.c
csmith > $CSMITH_OUT
${DIR_NAME}/instrument.sh $CSMITH_OUT $1
#rm $CSMITH_OUT
