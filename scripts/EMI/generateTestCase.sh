#!/bin/bash
DIR_NAME=$(dirname "$(realpath $0)")
source $DIR_NAME/../settings.sh
INSTRUMENT_TOOL=$GH_PROJECT_ROOT/src2src/build/tool
CSMITH_OUT=csmith$1
TMP_FILE=$(mktemp)
csmith -o $1 &&\
$INSTRUMENT_TOOL -extra-arg="-I$CSMITH_RUNTIME"  $1 --  > $TMP_FILE 
mv $TMP_FILE $1
rm $TMP_FILE
