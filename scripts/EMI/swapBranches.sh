#!/bin/bash
DIR_NAME=$(dirname "$(realpath $0)")
source $DIR_NAME/../settings.sh
INSTRUMENT_TOOL=$GH_PROJECT_ROOT/src2src/build/tool
$INSTRUMENT_TOOL -extra-arg="-I$INST_LIB_PATH" -extra-arg="-I$CSMITH_RUNTIME" -swapBranches $1 --  > $2 
