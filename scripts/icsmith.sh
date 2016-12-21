#!/bin/bash
#instrumented csmith
#runs csmith and instruments the output
#USAGE: ./icsmith filename.c

if [ -z $1 ];
then
	echo "USAGE: ./icsmith filename.c"
	exit 1	
fi

TEMP_FILE=$(mktemp)
ERR_FILE=$(mktemp)
DIR_NAME=$(dirname "$(realpath $0)")
SEED="-s 3601711868"
csmith $SEED > $1
${DIR_NAME}/instrument.sh $1 $TEMP_FILE 2> $ERR_FILE &&\
mv $TEMP_FILE $1 &&\
rm $ERR_FILE &&\
exit 0 || echo -e "FAILED to instrument, error file:\n$ERR_FILE"
exit 1
