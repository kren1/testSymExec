#!/bin/bash
#instrumented csmith
#runs csmith and instruments the output
#USAGE: ./icsmith filename.c

TEMP_FILE=$(mktemp)
DIR_NAME=$(dirname "$(realpath $0)")
csmith > $1
${DIR_NAME}/instrument.sh $1 $TEMP_FILE
mv $TEMP_FILE $1
#rm $CSMITH_OUT
exit 0
