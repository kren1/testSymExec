#!/bin/bash
DIR_NAME=$(dirname "$(realpath $0)")
source $DIR_NAME/settings.sh
INSTRUMENT_TOOL=$GH_PROJECT_ROOT/src2src/build/tool
$INSTRUMENT_TOOL -extra-arg="-I$CSMITH_RUNTIME" -deadCond $1 --  > $2 
